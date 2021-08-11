#include "Factory.h"

Tool* Factory(std::string tool) {
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="ACC_DataRead") ret=new ACC_DataRead;
if (tool=="ACC_Receive") ret=new ACC_Receive;
if (tool=="ACC_SetupBoards") ret=new ACC_SetupBoards;
if (tool=="ACC_Stream") ret=new ACC_Stream;
if (tool=="DummyTool") ret=new DummyTool;
if (tool=="SC_Emergency") ret=new SC_Emergency;
if (tool=="SC_Poll") ret=new SC_Poll;
if (tool=="SC_Receive") ret=new SC_Receive;
if (tool=="SC_SetConfig") ret=new SC_SetConfig;
if (tool=="SC_Stream") ret=new SC_Stream;

return ret;
}
