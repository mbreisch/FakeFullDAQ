#ifndef PsecConfig_H
#define PsecConfig_H

#include "zmq.hpp"
#include <SerialisableObject.h>
#include <iostream>
#include <vector>

class PsecConfig : public SerialisableObject{

 friend class boost::serialization::access;

 public:

  PsecConfig();
  
  //Comms
  bool Send(zmq::socket_t* sock);
  bool Receive(zmq::socket_t* sock);
 
  unsigned int VersionNumber = 0x0001;
  int receiveFlag = 0;

  //Reset switch
  int ResetSwitchACC = 0;
  int ResetSwitchACDC = 0;
 
  //trigger
  int triggermode = 0;

  //triggersettings
  int ACC_Sign = 0;
  int ACDC_Sign = 0;
  int SELF_Sign = 0;
  int SELF_Enable_Coincidence = 0;
  int SELF_Coincidence_Number = 0;
  int SELF_threshold = 0;

  //ACDC boards
  unsigned int ACDC_mask  = 0x00;

  //PSEC chips for self trigger
  int PSEC_Chip_Mask_0 = 0;
  int PSEC_Chip_Mask_1 = 0;
  int PSEC_Chip_Mask_2 = 0;
  int PSEC_Chip_Mask_3 = 0;
  int PSEC_Chip_Mask_4 = 0;
  unsigned int PSEC_Channel_Mask_0 = 0x00;
  unsigned int PSEC_Channel_Mask_1 = 0x00;
  unsigned int PSEC_Channel_Mask_2 = 0x00;
  unsigned int PSEC_Channel_Mask_3 = 0x00;
  unsigned int PSEC_Channel_Mask_4 = 0x00;

  //Validation time
  float Validation_Start = 0;
  float Validation_Window = 0;

  //Calibration mode
  int Calibration_Mode = 0;

  //Raw mode
  bool Raw_Mode = false;

  //Pedestal set value channel
  int Pedestal_channel = 0;
  unsigned int Pedestal_channel_mask = 0x00;
 
  //PPS settings
  unsigned int PPSRatio = 0x00;
  int PPSBeamMultiplexer = 0;

  bool SetDefaults();
  bool Print();

 private:
 
 template <class Archive> void serialize(Archive& ar, const unsigned int version){

  ar & receiveFlag;
  ar & ResetSwitchACC;
  ar & ResetSwitchACDC;
  ar & triggermode;
  ar & ACC_Sign;
  ar & ACDC_Sign;
  ar & SELF_Sign;
  ar & SELF_Enable_Coincidence;
  ar & SELF_Coincidence_Number;
  ar & SELF_threshold;

  //ACDC boards
  ar & ACDC_mask;

  //PSEC chips for self trigger
  ar & PSEC_Chip_Mask_0;
  ar & PSEC_Chip_Mask_1;
  ar & PSEC_Chip_Mask_2;
  ar & PSEC_Chip_Mask_3;
  ar & PSEC_Chip_Mask_4;
  ar & PSEC_Channel_Mask_0;
  ar & PSEC_Channel_Mask_1;
  ar & PSEC_Channel_Mask_2;
  ar & PSEC_Channel_Mask_3;
  ar & PSEC_Channel_Mask_4;

  //Validation time
  ar & Validation_Start;
  ar & Validation_Window;

  //Calibration mode
  ar & Calibration_Mode;

  //Raw mode
  ar & Raw_Mode;

  //Pedestal set value channel
  ar & Pedestal_channel;
  ar & Pedestal_channel_mask;
  
  //PPS setting
  ar & PPSRatio;
  ar & PPSBeamMultiplexer;
 }
 
};

#endif
