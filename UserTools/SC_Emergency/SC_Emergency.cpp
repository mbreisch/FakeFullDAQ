#include "SC_Emergency.h"

SC_Emergency::SC_Emergency():Tool(){}


bool SC_Emergency::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;


  return true;
}


bool SC_Emergency::Execute(){

  return true;
}


bool SC_Emergency::Finalise(){

  return true;
}
