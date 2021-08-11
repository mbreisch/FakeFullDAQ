#include "SC_Poll.h"

SC_Poll::SC_Poll():Tool(){}


bool SC_Poll::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

  return true;
}


bool SC_Poll::Execute(){

  //LV
  //nt LVstate = m_data->CB->GetLV_ONOFF();
  m_data->SCMonitor.LV_mon = (int)round(Random01());

  
  //std::vector<float> LVvoltage = m_data->CB->GetLV_voltage();
  m_data->SCMonitor.v33 = 3.25 + Random01()*0.1;
  m_data->SCMonitor.v25 = 2.45 + Random01()*0.1;
  m_data->SCMonitor.v12 = 1.15 + Random01()*0.1;

  //HV
  //int HVstate = m_data->CB->GetHV_ONOFF();
  m_data->SCMonitor.HV_mon = (int)round(Random01());


  //RHT
  //vector<float> RHT = m_data->CB->GetTemp();
  m_data->SCMonitor.temperature_mon = 15+Random01()*10;
  m_data->SCMonitor.humidity_mon = 30+Random01()*10;
  
  //DAC0
  m_data->SCMonitor.Trig0_mon = 3.25 + Random01()*0.1;

  //DAC1
  m_data->SCMonitor.Trig1_mon = 3.25 + Random01()*0.1;

  //Relay
  m_data->SCMonitor.relayCh1_mon = (int)round(Random01());
  m_data->SCMonitor.relayCh2_mon = (int)round(Random01());
  m_data->SCMonitor.relayCh3_mon = (int)round(Random01());
  
  //Photodiode
  m_data->SCMonitor.light = Random01();

  m_data->SCMonitor.errorcodes = {0x0};

  return true;
}


bool SC_Poll::Finalise(){

  m_data->CB->Disconnect();
  delete m_data->CB;
  m_data->CB=0;

  return true;
}

float SC_Poll::Random01(){
  auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
  std::mt19937 generator((unsigned)dt);
  std::uniform_real_distribution<float> dis(0.0, 1.0);

  float randomRealBetweenZeroAndOne = dis(generator);

  return randomRealBetweenZeroAndOne;
}