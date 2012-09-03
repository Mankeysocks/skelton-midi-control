//
//  RtMidiEnumerations.h
//  SkeletonMidiController
//
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleTest_RtMidiEnumerations_h
#define SkeletonModuleTest_RtMidiEnumerations_h

namespace rt {
    
    //
    // The first 4-bits of the first byte are the command name...
    //
    namespace CommandName{
        enum CommandName{
            NoteOff = 0x80, // note, velocity
            NoteOn = 0x90, // note, velocity
            AfterTouch = 0xA0, // note, value
            ControlChange = 0xB0, // controller, value
            PatchChange = 0xC0, // program
            ChannelPressure = 0xD0,
            PitchBend = 0xE0,
            SystemMessage = 0xF0,
        };
    }
    
    //
    // ...followed by either a channel number
    //
    namespace Channel{
        enum Channel{
            
            Channel1 = 0x0,
            Channel2 = 0x1,
            Channel3 = 0x2,
            Channel4 = 0x3,
            Channel5 = 0x4,
            Channel6 = 0x5,
            Channel7 = 0x6,
            Channel8 = 0x7,
            Channel9 = 0x8,
            Channel10 = 0x9,
            Channel11 = 0xA,
            Channel12 = 0xB,
            Channel13 = 0xC,
            Channel14 = 0xD,
            Channel15 = 0xE,
            Channel16 = 0xF,
            
        };
    }
    // or a system message type (if command name is system message)
    namespace SystemMessageType{
        enum SystemMessageType{
            // System messages
            SystemExclusive = 0x0,
            TimeCode = 0x1,             //MTC
            SongPositionPointer = 0x2,  //
            
            
            SongSelect = 0x3,
            TuneRequest = 0x6,
            EndOfSysEx = 0x7,
            // realtime messages
            Clock = 0x8,
            Start = 0xA,
            Continue = 0xB,
            Stop = 0xC,
            
            ActiveSensing = 0xE,
            SystemReset = 0xF,
        };
    }
    
    
    //
    // followed by a pitch
    //
    namespace Pitch{
        enum Pitch{
            
            CMinus1         = 0x00,
            CSharpMinus1    = 0x01,
            DMinus1         = 0x02,
            DSharpMinus1    = 0x03,
            EMinus1         = 0x04,
            FMinus1         = 0x05,
            FSharpMinus1    = 0x06,
            GMinus1         = 0x07,
            GSharpMinus1    = 0x08,
            AMinus1         = 0x09,
            ASharpMinus1    = 0x0A,
            BMinus1         = 0x0B,
            C0              = 0x0C,
            CSharp0         = 0x0D,
            D0              = 0x0E,
            DSharp0         = 0x0F,
            E0              = 0x10,
            F0              = 0x11,
            FSharp0         = 0x12,
            G0              = 0x13,
            GSharp0         = 0x14,
            A0              = 0x15,
            ASharp0         = 0x16,
            B0              = 0x17,
            C1              = 0x18,
            CSharp1         = 0x19,
            D1              = 0x1A,
            DSharp1         = 0x1B,
            E1              = 0x1C,
            F1              = 0x1D,
            FSharp1         = 0x1E,
            G1              = 0x1F,
            GSharp1         = 0x20,
            A1              = 0x21,
            ASharp1         = 0x22,
            B1              = 0x23,
            C2              = 0x24,
            CSharp2         = 0x25,
            D2              = 0x26,
            DSharp2         = 0x27,
            E2              = 0x28,
            F2              = 0x29,
            FSharp2         = 0x2A,
            G2              = 0x2B,
            GSharp2         = 0x2C,
            A2              = 0x2D,
            ASharp2         = 0x2E,
            B2              = 0x2F,
            C3              = 0x30,
            CSharp3         = 0x31,
            D3              = 0x32,
            DSharp3         = 0x33,
            E3              = 0x34,
            F3              = 0x35,
            FSharp3         = 0x36,
            G3              = 0x37,
            GSharp3         = 0x38,
            A3              = 0x39,
            ASharp3         = 0x3A,
            B3              = 0x3B,
            C               = 0x3C,
            CSharp4         = 0x3D,
            D4              = 0x3E,
            DSharp4         = 0x3F,
            E4              = 0x40,
            F4              = 0x41,
            FSharp4         = 0x42,
            G4              = 0x43,
            GSharp4         = 0x44,
            A4              = 0x45,
            ASharp4         = 0x46,
            B4              = 0x47,
            C5              = 0x48,
            CSharp5         = 0x49,
            D5              = 0x4A,
            DSharp5         = 0x4B,
            E5              = 0x4C,
            F5              = 0x4D,
            FSharp5         = 0x4E,
            G5              = 0x4F,
            GSharp5         = 0x50,
            A5              = 0x51,
            ASharp5         = 0x52,
            B5              = 0x53,
            C6              = 0x54,
            CSharp6         = 0x55,
            D6              = 0x56,
            DSharp6         = 0x57,
            E6              = 0x58,
            F6              = 0x59,
            FSharp6         = 0x5A,
            G6              = 0x5B,
            GSharp6         = 0x5C,
            A6              = 0x5D,
            ASharp6         = 0x5E,
            B6              = 0x5F,
            C7              = 0x60,
            CSharp7         = 0x61,
            D7              = 0x62,
            DSharp7         = 0x63,
            E7              = 0x64,
            F7              = 0x65,
            FSharp7         = 0x66,
            G7              = 0x67,
            GSharp7         = 0x68,
            A7              = 0x69,
            ASharp7         = 0x6A,
            B7              = 0x6B,
            C8              = 0x6C,
            CSharp8         = 0x6D,
            D8              = 0x6E,
            DSharp8         = 0x6F,
            E8              = 0x70,
            F8              = 0x71,
            FSharp8         = 0x72,
            G8              = 0x73,
            GSharp8         = 0x74,
            A8              = 0x75,
            ASharp8         = 0x76,
            B8              = 0x77,
        };
    }
    // or a control number
    namespace Control{
        enum Control {
            
            BankSelect						=0x00,
            ModWheel						=0x01,
            BreathController				=0x02,
            FootController					=0x04,
            PortamentoTime					=0x05,
            DataEntryMSB					=0x06,
            Volume							=0x07,
            Balance							=0x08,
            Pan								=0x0A,
            ExpressionController			=0x0B,
            GeneralPurpose1					=0x10,
            GeneralPurpose2					=0x11,
            GeneralPurpose3					=0x12,
            GeneralPurpose4					=0x13,
            Sustain							=0x40,
            Portamento						=0x41,
            Sustenuto						=0x42,
            SoftPedal						=0x43,
            Hold2							=0x45,
            GeneralPurpose5					=0x50,
            TempChange /*General Purpose 6*/=0x51,      
            GeneralPurpose6					=0x51,
            GeneralPurpose7					=0x52,
            GeneralPurpose8					=0x53,
            ExtEffectsDepth					=0x5B,
            TremeloDepth					=0x5C,
            ChorusDepth						=0x5D,
            DetuneDepth /*Celeste Depth*/	=0x5E,
            PhaserDepth						=0x5F,
            DataIncrement /*Data Entry +1*/	=0x60,
            DataDecrement /*Data Entry -1*/	=0x61,
            NonRegisteredParamLSB			=0x62,
            NonRegisteredParamMSB			=0x63,
            RegisteredParamLSB				=0x64,      
            RegisteredParamMSB				=0x65,      
            ResetAllControllers				=0x79,       //Val ??
            LocalControl					=0x7A,       //Val 0 = off, 7F (127) = on
            AllNotesOff						=0x7B,       //Val must be 0
            OmniModeOff						=0x7C,       //Val must be 0
            OmniModeOn						=0x7D,       //Val must be 0
            MonoModeOn						=0x7E,       //Val = # of channels, or 0 if # channels equals # voices in receiver
            PolyModeOn						=0x7F,         //Val must be 0
        };
    }
}

#endif
