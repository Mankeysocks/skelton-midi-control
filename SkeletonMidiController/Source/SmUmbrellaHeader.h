// Master header file: SmUmbrellaHeader.h

#define SM_SRCROOT "/Users/brocktane/Documents/Projects/SkeletonMidiController/SkeletonMidiController"

#define SAMPLE_XML_PATH SM_SRCROOT "/Resources/SamplesConfig.xml"
#define PITCH_GRAPH_C SM_SRCROOT "/Resources/IVviIVmedly - I.txt"
#define PITCH_GRAPH_G SM_SRCROOT "/Resources/IVviIVmedly - V.txt"
#define PITCH_GRAPH_A SM_SRCROOT "/Resources/IVviIVmedly - vi.txt"
#define PITCH_GRAPH_F SM_SRCROOT "/Resources/IVviIVmedly - IV.txt"
#define PITCH_GRAPH_Bass SM_SRCROOT "/Resources/IVviIVmedly - bass.txt"

#ifdef __MACH__
#define __MACOSX_CORE__
#define __SM_MACOSX__

#else
#ifdef __WIN32__
//#define SAMPLE_XML_PATH "C:\\Program Files (x86)\\OpenNI\\Data\\SamplesConfig.xml"
#define __WINDOWS_MM__
#define __SM_WIN32__
#endif
#endif

#define __SM_DEBUG_MODE__
#define __SM_DEBUG_SUBTLE__
#define __SM_DEBUG_PARSE__
//#define __SM_DEBUG_CALLBACKS__

#define SM_NUM_USERS_TO_TRACK 8
#define SM_BUFFER_SIZE 256
#define SM_MIDI_INPUT_PORT_NAME "IAC Driver Designated SMC"
#define SM_MIDI_OUTPUT_PORT_NAME "IAC Driver Designated Logic"