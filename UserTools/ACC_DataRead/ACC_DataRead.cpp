#include "ACC_DataRead.h"

ACC_DataRead::ACC_DataRead():Tool(){}


bool ACC_DataRead::Initialise(std::string configfile, DataModel &data){

	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();

	m_data= &data;
	m_log= m_data->Log;

	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

	//Fake Data
	///Load Raw Board 1
	cout << " Reading Board 1";
	m_variables.Get("filenameB1",fileB1);
	LoadFile(fileB1,1,0);
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileData[0].size() << endl;

	///Load Raw Board 2
	cout << " Reading Board 2";
	m_variables.Get("filenameB2",fileB2);
	LoadFile(fileB2,1,1);	
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileData[1].size() << endl;

	///Load Raw Board 3
	cout << " Reading Board 3";
	m_variables.Get("filenameB3",fileB3);
	LoadFile(fileB3,1,2);	
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileData[2].size() << endl;

	///Load Raw Board 4
	cout << " Reading Board 4";
	m_variables.Get("filenameB4",fileB4);
	LoadFile(fileB4,1,3);
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileData[3].size() << endl;

	//Fake ACC
	cout << " Reading ACC Frame";
	m_variables.Get("filenameACC",fileACC);
	LoadFile(fileACC,2,0);
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileACC.size() << endl;

	//Fake Error 
	cout << " Reading Errors";
	m_variables.Get("filenameError",fileError);
	LoadFile(fileError,3,0);
	cout << "... done" << endl; 
	cout << "Got " << m_data->TCS.ReadFileError.size() << endl;

	return true;
}


bool ACC_DataRead::Execute(){

	//cout << "Gave " << m_data->TCS.ReadFileData.size() << " Boards with " << m_data->TCS.ReadFileData[0].size() << " each to DATA" << endl;
	m_data->psec.ReceiveData = m_data->TCS.ReadFileData;

	//cout << "Gave " << m_data->TCS.ReadFileACC.size() << " to ACC" << endl;
	m_data->psec.AccInfoFrame = m_data->TCS.ReadFileACC;

	if(Random01() < 0.01)
	{
		//cout << "Gave " << m_data->TCS.ReadFileError.size() << " to ERROR" << endl;
		m_data->psec.errorcodes = m_data->TCS.ReadFileError;	
	}else
	{
		m_data->psec.errorcodes = {0x00000000};
	}

	return true;
}


bool ACC_DataRead::Finalise(){
	return true;
}

bool ACC_DataRead::LoadFile(string filename, int formattype, int bi)
{
	ifstream rfile;
	rfile.open(filename);

  string line;
  while(getline(rfile,line))
  {
    if(formattype==1)
    {
    	//cout << "Line " << line << " to " << (unsigned short)stoul(line,0,16) << endl;
    	m_data->TCS.ReadFileData[bi].push_back((unsigned short)stoul(line,0,16));
    }else if(formattype==2)
    {
    	m_data->TCS.ReadFileACC.push_back((unsigned short)stoul(line,0,16));
    }else if(formattype==3)
    {
    	m_data->TCS.ReadFileError.push_back((unsigned int)stoul(line,0,16));
    }
  }
  rfile.close();

  return true;
}

float ACC_DataRead::Random01(){
	auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
	std::mt19937 generator((unsigned)dt);
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float randomRealBetweenZeroAndOne = dis(generator);

	return randomRealBetweenZeroAndOne;
}
