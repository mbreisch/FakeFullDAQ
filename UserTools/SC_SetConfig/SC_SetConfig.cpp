#include "SC_SetConfig.h"

SC_SetConfig::SC_SetConfig():Tool(){}


bool SC_SetConfig::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;
 
  m_data->CB= new Canbus();
  m_data->CB->Connect(); 
	
  std::fstream infile("./configfiles/SlowControl/LastHV.txt", std::ios_base::in);
  if(infile.is_open())
  {
  	infile >> m_data->CB->get_HV_volts;
  	infile.close();
  }
	
  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  return true;
}


bool SC_SetConfig::Execute(){
  int retval;
  //check LV/HV state_set 
  if(m_data->SCMonitor.recieveFlag==0)
  {
    bool temp_HVS;
    m_variables.Get("HV_state_set",temp_HVS);
    m_data->SCMonitor.HV_state_set = temp_HVS;

    bool temp_LVS;
    m_variables.Get("LV_state_set",temp_LVS);
    m_data->SCMonitor.LV_state_set = temp_LVS;

    float temp_HVV;
    m_variables.Get("HV_volts",temp_HVV);
    m_data->SCMonitor.HV_volts = temp_HVV;

    float temp_Tr1;
    m_variables.Get("Trig1_threshold",temp_Tr1);
    m_data->SCMonitor.Trig1_threshold = temp_Tr1;

    float temp_Tr0;
    m_variables.Get("Trig0_threshold",temp_Tr0);
    m_data->SCMonitor.Trig0_threshold = temp_Tr0;

    float temp_VREF;
    m_variables.Get("TrigVref",temp_VREF);
    m_data->SCMonitor.TrigVref = temp_VREF;

    bool temp_r1, temp_r2,temp_r3;
    m_variables.Get("relayCh1",temp_r1);
    m_data->SCMonitor.relayCh1 = temp_r1;
    m_variables.Get("relayCh2",temp_r2);
    m_data->SCMonitor.relayCh2 = temp_r2;
    m_variables.Get("relayCh3",temp_r3);
    m_data->SCMonitor.relayCh3 = temp_r3;
    
    m_data->SCMonitor.relayCh1_mon = m_data->CB->GetRelayState(1);
    m_data->SCMonitor.relayCh2_mon = m_data->CB->GetRelayState(2);
    m_data->SCMonitor.relayCh3_mon = m_data->CB->GetRelayState(3);
  }
	
  if(m_data->SCMonitor.recieveFlag==0 || m_data->SCMonitor.recieveFlag==1)
  {
    //------------------------------------Relay Control
    if(m_data->SCMonitor.relayCh1!=m_data->SCMonitor.relayCh1_mon)
    {
      retval = m_data->CB->SetRelay(1,m_data->SCMonitor.relayCh1);
      if(retval!=0 && retval!=1)
      {
        std::cout << " There was an error (Relay 1) with retval: " << retval << std::endl;
      }
    }

    if(m_data->SCMonitor.relayCh2!=m_data->SCMonitor.relayCh2_mon)
    {
      retval = m_data->CB->SetRelay(2,m_data->SCMonitor.relayCh2);
      if(retval!=0 && retval!=1)
      {
        std::cout << " There was an error (Relay 2) with retval: " << retval << std::endl;
      }
    }

    if(m_data->SCMonitor.relayCh3!=m_data->SCMonitor.relayCh3_mon)
    {
      retval = m_data->CB->SetRelay(3,m_data->SCMonitor.relayCh3);
      if(retval!=0 && retval!=1)
      {
        std::cout << " There was an error (Relay 3) with retval: " << retval << std::endl;
      }
    }  
    
    
    //------------------------------------LV Control
    bool tempLVmon;
    int tCB_LV = m_data->CB->GetLV_ONOFF();
    if(tCB_LV==0)
    {
      tempLVmon = false;
      m_data->SCMonitor.LV_mon = 0;
    }else if(tCB_LV==1)
    {
      tempLVmon = true;
      m_data->SCMonitor.LV_mon = 1;
    }     
    if(m_data->SCMonitor.LV_state_set!=tempLVmon)
    {
  	  retval = m_data->CB->SetLV(m_data->SCMonitor.LV_state_set);
      if(retval!=0 && retval!=1)
  	  {
  		  std::cout << " There was an error (Set LV) with retval: " << retval << std::endl;
  	  }
    }
    
    
    //------------------------------------HV Control
    bool tempHVmon;
    int tCB_HV = m_data->CB->GetHV_ONOFF();
    if(tCB_HV==0)
    {
      tempHVmon = false;
      m_data->SCMonitor.HV_mon = 0;
    }else if(tCB_HV==1)
    {
      tempHVmon = true;
      m_data->SCMonitor.HV_mon = 1;
    }  
    if(m_data->SCMonitor.HV_state_set!=tempHVmon)
    {
      retval = m_data->CB->SetHV_ONOFF(m_data->SCMonitor.HV_state_set);
      if(retval!=0 && retval!=1)
      {
        std::cout << " There was an error (Set HV) with retval: " << retval << std::endl;
      }
    }

    if(m_data->SCMonitor.HV_volts!=m_data->CB->get_HV_volts)
    {
      retval = m_data->CB->SetHV_voltage(m_data->SCMonitor.HV_volts);
      if(retval==0)
      {
        m_data->CB->get_HV_volts = m_data->SCMonitor.HV_volts;
        std::fstream outfile("./configfiles/SlowControl/LastHV.txt", std::ios_base::out | std::ios_base::trunc);
        outfile << m_data->CB->get_HV_volts;
        outfile.close();
      }else
      {
        std::cout << " There was an error (HV V set) with retval: " << retval << std::endl;
      }
    }


    //------------------------------------Triggerboard Control
    float tempval;
    if(m_data->SCMonitor.Trig0_threshold!=m_data->CB->GetTriggerDac0(m_data->SCMonitor.TrigVref))
    {
	retval = m_data->CB->SetTriggerDac0(m_data->SCMonitor.Trig0_threshold, m_data->SCMonitor.TrigVref);
	if(retval!=0)
	{
		std::cout << " There was an error (DAC0) with retval: " << retval << std::endl;
	}
	tempval = m_data->CB->GetTriggerDac0(m_data->SCMonitor.TrigVref);
	if(std::abs(tempval - m_data->SCMonitor.Trig0_threshold)<0.000000001)
	{
		m_data->SCMonitor.Trig0_mon = tempval;
	}else
	{
		std::cout << " There was an error (DAC0) - 0xC0 hasn't been updated!" << std::endl;
	}
    }

    if(m_data->SCMonitor.Trig1_threshold!=m_data->CB->GetTriggerDac0(m_data->SCMonitor.TrigVref))
    {
	retval = m_data->CB->SetTriggerDac1(m_data->SCMonitor.Trig1_threshold, m_data->SCMonitor.TrigVref);
	if(retval!=0)
	{
		std::cout << " There was an error (DAC1) with retval: " << retval << std::endl;
	}
	tempval = m_data->CB->GetTriggerDac1(m_data->SCMonitor.TrigVref);
	if(std::abs(tempval - m_data->SCMonitor.Trig1_threshold)<0.000000001)
	{
		m_data->SCMonitor.Trig1_mon = tempval;
	}else
	{
		std::cout << " There was an error (DAC1) - 0xC0 hasn't been updated!" << std::endl;
	}
    } 
    
    m_data->SCMonitor.recieveFlag=2;
  }
  return true;
}


bool SC_SetConfig::Finalise(){

  m_data->CB->get_HV_volts = 0;
  return true;
}
