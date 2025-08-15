using Blind_Config_Tool.RedesignFiles;
using System.Collections.Generic;

namespace Blind_Config_Tool.Core
{
    public class MessageScripts
    {
        #region Sending/Receiving Messages
        //Defining Messages that will be sent (also define the message type that will be sent back)

        /// <summary>
        /// MOTOR MESSAGES
        /// </summary>

        //SEARCH FOR MOTORS//
        //private static CommunicationAction _searchForMotors_Part1 = new CommunicationAction( //Prepares the system for a search message
        //    /*Element action*/ ElementActions.DO_NOTHING,
        //    /*Target*/ Target.BROADCAST_ALL,
        //    /*Preprocess function*/ PreprocessActions.Preprocess_Add_Motors_To_List,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x56 },
        //        /*Length*/ new byte[] { 0x12 },
        //        /*Data*/ new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF }
        //    ),
        //    /*Description*/ "Searching for motors"
        //);

        //private static CommunicationAction _searchForMotors_Part2 = new CommunicationAction(    //Validation message on motors that have just responded to the search message
        //    ElementActions.DO_NOTHING,
        //    Target.BROADCAST_ALL,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x50 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { 0x00 }
        //    ),
        //    /*Description*/ "Searching for motors"
        //);

        private static CommunicationAction _searchForMotors_Part3 = new CommunicationAction( //Message that causes the motors to respond with their addresses
            ElementActions.MOTOR_SEARCH,
            /*Target*/ Target.BROADCAST_ALL,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_ID },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Searching for motors",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_ID },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response Function*/ ResponseActions.Response_Add_Motors_To_List,
            /*Requires ack*/true
        );

        private static CommunicationAction _validateMotors = new CommunicationAction(    //Validates a single motor
            ElementActions.MOTOR_VALIDATE,
            Target.ALL_MOTORS_IN_LIST,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.FLOW_VALIDATE_DEVICE },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Validating motors",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK, (byte)ScriptCommands.NACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response Function*/ ResponseActions.Response_Validate_Motor,
            /*Requires ack*/true
        );

        private static CommunicationAction _getAllMotorLabels = new CommunicationAction(    //Getting the labels for all motors on the network
            ElementActions.MOTOR_GET_NAME,
            /*Target*/ Target.ALL_MOTORS_IN_LIST,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_LABEL },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Retrieving motor labels",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_LABEL },
            /*Response Length*/ new byte[] { 0x1C },
            /*Response Function*/ ResponseActions.Response_Get_Motor_Label,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _getSingleMotorLabel = new CommunicationAction(  //Getting a label for the selected motor
            ElementActions.MOTOR_GET_NAME,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_LABEL },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Retrieving motor label",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_LABEL },
            /*Response Length*/ new byte[] { 0x1C },
            /*Response Function*/ ResponseActions.Response_Get_Motor_Label,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _getGroupIDs = new CommunicationAction(  //Getting all of the group IDs for the selected motor
            ElementActions.MOTOR_GET_GROUP_IDS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/PreprocessActions.Preprocess_Get_Group_IDs,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_GROUP_ID },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { 0xFF }
            ),
            /*Description*/ "Retrieving group IDs",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_GROUP_ID },
            /*Response Length*/ new byte[] { 0x0F },
            /*Response function*/ResponseActions.Response_Get_Group_IDs,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _getInternalPositions = new CommunicationAction(  //Getting all of the internal positions for the selected motor
            ElementActions.MOTOR_GET_INTERNAL_POSITIONS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/PreprocessActions.Preprocess_Get_Internal_Positions,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_INTERNAL_POSITION },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { 0xFF }
            ),
            /*Description*/ "Retrieving internal positions",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_INTERNAL_POSITION },
            /*Response Length*/ new byte[] { 0x0F, 0x14, 0x13 },
            /*Response function*/ResponseActions.Response_Get_Internal_Positions,   
            /*Requires ack*/ true
        );

        private static CommunicationAction _getMotorOrientation = new CommunicationAction(  //Getting the orientation of the selected motor (whether it runs forward or reverse)
            ElementActions.MOTOR_GET_ORIENTATION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_DIRECTION },
                /*Length*/ new byte[] { 0x8B },
                /*Data*/ new byte[] { 0xFF }
            ),
            /*Description*/ "Retrieving motor orientation",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_DIRECTION },
            /*Response Length*/ new byte[] { 0x0C, (byte)ScriptCommands.NACK},
            /*Response function*/ ResponseActions.Response_Get_Motor_Orientation,
            /*Requires ack*/true
        );

        private static CommunicationAction _getMotorLimits = new CommunicationAction(  //Getting the up and down limits of the selected motor
            ElementActions.MOTOR_GET_LIMITS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_LIMITS },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Retrieving motor up/down limits",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_LIMITS },
            /*Response Length*/ new byte[] { 0x0F },

            /*Response function*/ ResponseActions.Response_Get_Motor_Limits,
            /*Requires ack*/ true
        );

        private static CommunicationAction _moveMotor_Up = new CommunicationAction(  //Moves the motor up
            ElementActions.MOTOR_MOVE_UP,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_MOVE },
                /*Length*/ new byte[] { 0x8E },
                /*Data*/ new byte[] { 0x01, 0x64, 0x02 }
            ),
            /*Description*/ "Moving motor up",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Move_Motor_Up,
            /*Requires ack*/ true
        );

        private static CommunicationAction _moveMotor_Down = new CommunicationAction(  //Moves the motor down
            ElementActions.MOTOR_MOVE_DOWN,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_MOVE },
                /*Length*/ new byte[] { 0x8E },
                /*Data*/ new byte[] { 0x02, 0x64, 0x02 }
            ),
            /*Description*/ "Moving motor down",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Move_Motor_Down,
            /*Requires ack*/ true
        );

        private static CommunicationAction _stopMotor = new CommunicationAction(  //Stops the motor
            ElementActions.MOTOR_STOP,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_STOP },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { 0xFF}
            ),
            /*Description*/ "Stopping",
            true
        );

        private static CommunicationAction _startIDMovement = new CommunicationAction(  //Starts an ID movement (motor shakes back and forth to let the user know which one is being targeted)
            ElementActions.MOTOR_START_ID_MOVEMENT,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_VISUAL_FEEDBACK },
                /*Length*/ new byte[] { 0x8B },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Performing visual feedback"
        );

        private static CommunicationAction _moveToUpLimit = new CommunicationAction(  //Moves the motor to the up limit
            ElementActions.MOTOR_MOVE_TO_UP_LIMIT,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_TO_PERCENT },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x00}
            ),
            /*Description*/ "Moving to position",
            /*Requires ack*/true
        );

        private static CommunicationAction _moveToDownLimit = new CommunicationAction(  //Moves the motor to the down limit
            ElementActions.MOTOR_MOVE_TO_DOWN_LIMIT,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_TO_PERCENT },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x64 }
            ),
            /*Description*/ "Moving to position",
            /*Requires ack*/true
        );

        private static CommunicationAction _moveToSelectedIP = new CommunicationAction(  //Moves the motor to the selected IP's position
            ElementActions.MOTOR_MOVE_TO_SELECTED_IP,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/PreprocessActions.Preprocess_Move_To_Selected_Ip,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_TO_INTERNAL_POS },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Moving to position",
            /*Requires ack*/true
        );

        private static CommunicationAction _moveToPosition = new CommunicationAction(  //Moves the motor to a percentage position
            ElementActions.MOTOR_MOVE_TO_POSITION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/PreprocessActions.Preprocess_Move_To_Target,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.CMD_TO_PERCENT },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Moving to position",
            /*Requires ack*/true
        );

        private static CommunicationAction _setLimit_Up = new CommunicationAction(  //Sets the up limit
            ElementActions.MOTOR_SET_UP_LIMIT,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_LIMITS },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x01, 0x01, 0x00, 0x00 }
            ),
            /*Description*/ "Setting up limit",
            /*Requires ack*/true
        );

        private static CommunicationAction _setLimit_Down = new CommunicationAction(  //Sets the down limit
            ElementActions.MOTOR_SET_DOWN_LIMIT,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_LIMITS },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x01, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Setting down limit",
            /*Requires ack*/true
        );

        private static CommunicationAction _getSpeeds = new CommunicationAction(  //Getting the up/down/slow speeds of the motor
            ElementActions.MOTOR_GET_SPEEDS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_SPEED },
                /*Length*/ new byte[] { 0x8B },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Getting speeds",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_SPEED },
            /*Response Length*/ new byte[] { 0x11 , 0x0E },
            /*Response function*/ ResponseActions.Response_Get_Speeds,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _setSpeeds = new CommunicationAction(  //Setting the up/down/slow speeds of the motor
            ElementActions.MOTOR_SET_SPEEDS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Speeds,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_SPEED },
                /*Length*/ new byte[] { 0x8E },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00}
            ),
            /*Description*/ "Setting speeds",
            /*Requires ack*/true
        );

        private static CommunicationAction _setGroupID = new CommunicationAction(  //Setting a specific group ID
            ElementActions.MOTOR_SET_GROUP_ID,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Group_ID,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_GROUP_ID },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0xFF, 0xFF, 0xFF, 0xFF}
            ),
            /*Description*/ "Setting group ID",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response Function*/ null,     
            /*Requires Ack*/ true
        );

        private static CommunicationAction _setInternalPosition = new CommunicationAction(  //Getting a specific internal position
            ElementActions.MOTOR_SET_INTERNAL_POSITION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Internal_Positions,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_INTERNAL_POSITION },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x03, 0x01, 0xFF, 0x00}
            ),
            /*Description*/ "Setting internal position",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ null,
        /*Requires Ack*/ true
        );

        private static CommunicationAction _setMotorLabel = new CommunicationAction(  //Setting the label of the selected motor
            ElementActions.MOTOR_SET_NAME,
            /*Target*/ Target.SPECIFIC_MOTOR,
            PreprocessActions.Preprocess_Set_Label,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_LABEL },
                /*Length*/ new byte[] { 0x9B },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Setting motor name",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ null,
            /*Requires ack*/true
        );

        private static CommunicationAction _setMotorOrientation = new CommunicationAction(  //Setting the motor orientation (whether it drives forward or reverse)
            ElementActions.MOTOR_SET_ORIENTATION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Orientation,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_DIRECTION },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Setting motor orientation",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_DIRECTION },
            /*Response Length*/ new byte[] { (byte)ScriptCommands.NACK },
            /*Response function*/ResponseActions.Response_Set_Orientation,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _factoryResetMotor = new CommunicationAction(  //Erase all settings from the motor (except address)
            ElementActions.MOTOR_FACTORY_RESET,
            /*Target*/ Target.SPECIFIC_MOTOR,
            new MessageStructure(
                /*Command*/ new byte[] { 0x1F },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Performing factory reset"
        );

        private static CommunicationAction _setNewMotorAddress = new CommunicationAction(  //Setting a new motor address
            ElementActions.MOTOR_SET_ADDRESS,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Motor_Address,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_ID },
                /*Length*/ new byte[] { 0x0D },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Setting new address",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ResponseActions.Response_Set_Motor_Address,
            /*Requires ack*/true
        );

        private static CommunicationAction _setIPToCurrentPosition = new CommunicationAction(  //Setting a new motor address
            ElementActions.MOTOR_SET_IP_TO_CURRENT_POSITION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/ PreprocessActions.Preprocess_Set_Ip_To_Current_Position,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_SET_INTERNAL_POSITION },
                /*Length*/ new byte[] { 0x8F },
                /*Data*/ new byte[] { 0x01, 0x01, 0x00, 0x00 }
            ),
            /*Description*/ "Setting internal position",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ null,
        /*Requires Ack*/ true
        );

        private static CommunicationAction _getCurrentPositionIP = new CommunicationAction(
            ElementActions.MOTOR_GET_SINGLE_INTERNAL_POSITION,
            /*Target*/ Target.SPECIFIC_MOTOR,
            /*Preprocess function*/PreprocessActions.Preprocess_Get_Single_Internal_Position,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.MOTOR_GET_INTERNAL_POSITION },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { 0xFF }
            ),
            /*Description*/ "Retrieving internal position",
            /*Response Type*/ new byte[] { (byte)ScriptCommands.MOTOR_POST_INTERNAL_POSITION },
            /*Response Length*/ new byte[] { 0x0F },
            /*Response function*/ResponseActions.Response_Get_Single_Internal_Position,
            /*Requires ack*/ true
        );

        /// <summary>
        /// KEYPAD MESSAGES
        /// </summary>

        private static CommunicationAction _setKeypadName = new CommunicationAction(  //Setting a new name for the selected keypad
            ElementActions.KEYPAD_SET_NAME,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            PreprocessActions.Preprocess_Set_Keypad_Name,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_SET_LABEL },
                /*Length*/ new byte[] { 0x9B },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Setting keypad name",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response Function*/ ResponseActions.Response_Set_Keypad_Name,
            /*Requires ack*/true
        );

        private static CommunicationAction _getAllKeypadNames = new CommunicationAction(  //Getting the names of all keypads in the list
            ElementActions.KEYPAD_GET_NAME,
            /*Target*/ Target.ALL_KEYPADS_IN_LIST,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_GET_LABEL },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Getting keypad names",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_POST_LABEL },
            /*Response Length*/ new byte[] { 0x1C },
            /*Response function*/ ResponseActions.Response_Get_Keypad_Name,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _getSingleKeypadName = new CommunicationAction(  //Getting the name of the selected keypad
            ElementActions.KEYPAD_GET_NAME,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_GET_LABEL },
                /*Length*/ new byte[] { 0x8C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Getting keypad name",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_POST_LABEL },
            /*Response Length*/ new byte[] { 0x1C },
            /*Response function*/ ResponseActions.Response_Get_Keypad_Name,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _searchForKeypads = new CommunicationAction(  //Search for all keypads on the network
            ElementActions.KEYPAD_SEARCH,
            Target.BROADCAST_ALL,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_GET_ID },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Searching for keypads",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_POST_ID },
            /*Response Length*/ new byte[] { 0x0C },
            ResponseActions.Response_Add_Keypad_To_List,
            /*Requires ack*/true
        );

        private static CommunicationAction _validateKeypads = new CommunicationAction(    //Validates a single motor
            ElementActions.KEYPAD_VALIDATE,
            Target.ALL_KEYPADS_IN_LIST,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.FLOW_VALIDATE_DEVICE },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Validating motors",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK, (byte)ScriptCommands.NACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response Function*/ ResponseActions.Response_Validate_Keypad,
            /*Requires ack*/true
        );

        private static CommunicationAction _setKeypadAddress = new CommunicationAction(  //Sets the keypad address
            ElementActions.KEYPAD_SET_ADDRESS,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            /*Preprocess function*/PreprocessActions.Preprocess_Set_Keypad_Address,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_SET_ID },
                /*Length*/ new byte[] { 0x8E },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00}
            ),
            /*Description*/ "Setting keypad address",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Set_Keypad_Address,
            /*Requires Ack*/ true
        );

        //private static CommunicationAction _startProgrammingKeypadButtons = new CommunicationAction(  //Prepares the keypad to have is buttons programmed
        //    ElementActions.DO_NOTHING,
        //    Target.SPECIFIC_KEYPAD,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x94 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { 0x01 }
        //    ),
        //    /*Description*/ "Programming keypad buttons"
        //);

        private static CommunicationAction _programKeypadCommand = new CommunicationAction(  //Programs a keypad button
            ElementActions.KEYPAD_SET_COMMANDS,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            PreprocessActions.Preprocess_Program_Keypad_Commands,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_SET_COMMAND },
                /*Length*/ new byte[] { 0x5E },
                /*Data*/ new byte[] { 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Programming keypad buttons (1 of 8)",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Program_Keypad_Commands,
            /*Requires Ack*/ true
        );

        private static CommunicationAction _programKeypadGroup = new CommunicationAction(  //Programs a keypad group
            ElementActions.KEYPAD_SET_GROUP_ADDRESSES,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            PreprocessActions.Preprocess_Program_Keypad_Groups,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_SET_GROUP_ID },
                /*Length*/ new byte[] { 0xA3 },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Programming keypad group",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Program_Keypad_Groups,
            /*Requires Ack*/ true
        );

        //private static CommunicationAction _startRetrievingKeypadCommand_Part1 = new CommunicationAction(  //Part 1 of retrieving keypad commands
        //    ElementActions.DO_NOTHING,
        //    /*Target*/ Target.SPECIFIC_KEYPAD,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x81 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { }
        //    ),
        //    /*Description*/ "Retrieving button data"
        //);

        //private static CommunicationAction _startRetrievingKeypadCommand_Part2 = new CommunicationAction(  //Part 2 of retrieving keypad commands
        //    ElementActions.DO_NOTHING,
        //    /*Target*/ Target.SPECIFIC_KEYPAD,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x70 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { }
        //    ),
        //    /*Description*/ "Retrieving button data"
        //);

        //private static CommunicationAction _startRetrievingKeypadCommand_Part3 = new CommunicationAction(  //Part 3 of retrieving keypad commands
        //    ElementActions.DO_NOTHING,
        //    /*Target*/ Target.SPECIFIC_KEYPAD,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0x7F },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { }
        //    ),
        //    /*Description*/ "Retrieving button data"
        //);

        private static CommunicationAction _startRetrievingKeypadCommand_Part4 = new CommunicationAction(  //Part 4 of retrieving keypad commands
            ElementActions.DO_NOTHING,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_SET_ID },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Retrieving button data"
        );

        private static CommunicationAction _getKeypadCommand = new CommunicationAction(  //Retrieving a keypad command
            ElementActions.KEYPAD_GET_COMMANDS,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            /*Preprocess function*/PreprocessActions.Preprocess_Retrieve_Keypad_Commands,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_GET_COMMAND },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Retrieving button data (1 of 8)",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_POST_COMMAND },
            /*Response Length*/ new byte[] { 0x18 },
            /*Response function*/ ResponseActions.Response_Retrieve_Keypad_Commands,
            /*Requires ack*/true
        );

        private static CommunicationAction _getKeypadGroupAddresses = new CommunicationAction(  //Getting the group addresses
            ElementActions.KEYPAD_GET_GROUP_ADDRESSES,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            /*Preprocess function*/ PreprocessActions.Preprocess_Retrieve_Group_Addresses,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_GET_GROUP_ID },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { 0x00 }
            ),
            /*Description*/ "Retrieving button data",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.KEYPAD_POST_GROUP_ID},
            /*Response Length*/ new byte[] { 0x23, 0x2A},
            /*Response function*/ ResponseActions.Response_Retrieve_Group_Addresses,
            /*Requires ack*/true
        );

        /// <summary>
        /// SERIAL MESSAGES
        /// </summary>

        //private static CommunicationAction _startSettingMAC = new CommunicationAction(  //Prepares the device to receive a MAC address
        //    ElementActions.DO_NOTHING,
        //    /*Target*/ Target.SPECIFIC_KEYPAD,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0xB7 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { }
        //    ),
        //    /*Description*/ "Setting MAC address"
        //);

        private static CommunicationAction _setMACAddress = new CommunicationAction(  //Sets the MAC address
            ElementActions.SET_MAC_ADDRESS,
            /*Target*/ Target.SPECIFIC_KEYPAD,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.SYS_SET_MAC_ADDRESS },
                /*Length*/ new byte[] { 0x11 },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
            ),
            /*Description*/ "Setting MAC Address"
        );

        //private static CommunicationAction _prepareIPAddress = new CommunicationAction(  //Prepares the device to set a new IP address
        //    ElementActions.DO_NOTHING,
        //    /*Target*/ Target.SPECIFIC_DEVICE,
        //    new MessageStructure(
        //        /*Command*/ new byte[] { 0xE0 },
        //        /*Length*/ new byte[] { 0x0C },
        //        /*Data*/ new byte[] { }
        //    ),
        //    /*Description*/ "Setting IP address"
        //);

        private static CommunicationAction _setIPAddress = new CommunicationAction(  //Sets the IP address
            ElementActions.SET_IP_ADDRESS,
            /*Target*/ Target.SPECIFIC_DEVICE,
            PreprocessActions.Preprocess_Set_Ip_Address,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.SYS_SET_IP_ADDRESS },
                /*Length*/ new byte[] { 0x16 },
                /*Data*/ new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
            ),
            /*Description*/ "Setting IP address",
            /*Requires ack*/ true
        );

        private static CommunicationAction _retrieveIPAddress = new CommunicationAction(  //Gets the IP address
            ElementActions.GET_IP_ADDRESS,
            /*Target*/ Target.SPECIFIC_DEVICE,
            new MessageStructure(
                /*Command*/ new byte[] { (byte)ScriptCommands.SYS_GET_IP_ADDRESS },
                /*Length*/ new byte[] { 0x0C },
                /*Data*/ new byte[] { }
            ),
            /*Description*/ "Retrieving IP settings",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.SYS_POST_IP_ADDRESS },
            /*Response Length*/ new byte[] { 0x17 },
            /*Response function*/ ResponseActions.Response_Retrieve_Ip_Address,
            /*Requires ack*/ true
        );

        private static CommunicationAction _sendIPListEntry = new CommunicationAction(
            ElementActions.SET_SERVER_IP,
            /*Target*/Target.SPECIFIC_DEVICE,
            PreprocessActions.Preprocess_Set_Server_Ip,
            new MessageStructure(
            /*Command*/ new byte[] { (byte)ScriptCommands.SYS_SET_SERVER_IP },
            /*Length*/ new byte[] { 0x0C },
            /*Data*/ new byte[] { }
            ),
            /*Description*/ "Setting server IP",
            /*Response Command*/ new byte[] { (byte)ScriptCommands.ACK },
            /*Response Length*/ new byte[] { 0x0C },
            /*Response function*/ ResponseActions.Response_Set_Server_Ip
        );
        #endregion

        #region Message Scripts
        private List<CommunicationAction> SearchForMotors = new List<CommunicationAction> { _searchForMotors_Part3, _validateMotors, _getAllMotorLabels, _getSpeeds, _getMotorOrientation, _getMotorLimits};  //Gets all motors on the network, stores their names and addresses in a list, then gets all the details of the first motor in the list
        private List<CommunicationAction> SearchForMotorsGroupsAndIPs = new List<CommunicationAction> { _searchForMotors_Part3, _validateMotors, _getAllMotorLabels, _getGroupIDs, _getInternalPositions };
        private List<CommunicationAction> GetMotorData = new List<CommunicationAction> { _getSingleMotorLabel, _getSpeeds, _getMotorLimits, _getMotorOrientation };
        private List<CommunicationAction> GetGroupsAndIPs = new List<CommunicationAction> { _getSingleMotorLabel, _getGroupIDs , _getInternalPositions};
        private List<CommunicationAction> MoveMotorUp = new List<CommunicationAction> { _moveMotor_Up }; //Moves the motor up
        private List<CommunicationAction> MoveMotorDown = new List<CommunicationAction> { _moveMotor_Down }; //Moves the motor down
        private List<CommunicationAction> StopMotorMovement = new List<CommunicationAction> { _stopMotor }; //Stop the motor
        private List<CommunicationAction> StartIDMovement = new List<CommunicationAction> { _startIDMovement }; //Starts the identification movement
        private List<CommunicationAction> MoveToPosition = new List<CommunicationAction> { _moveToPosition };   //Move to the motor position
        private List<CommunicationAction> MoveToUpLimit = new List<CommunicationAction> { _moveToUpLimit };
        private List<CommunicationAction> MoveToDownLimit = new List<CommunicationAction> { _moveToDownLimit };
        private List<CommunicationAction> MoveToSelectedIP = new List<CommunicationAction> { _moveToSelectedIP };
        private List<CommunicationAction> FactoryResetMotor = new List<CommunicationAction> { _factoryResetMotor, _getSingleMotorLabel, _getSpeeds, _getMotorLimits, _getMotorOrientation, _getSingleMotorLabel }; //Performs a factory reset
        private List<CommunicationAction> SetNewFAMotorAddress = new List<CommunicationAction> { _setNewMotorAddress };
        private List<CommunicationAction> SetUpLimit = new List<CommunicationAction> { _setLimit_Up, _getMotorLimits };  //Sets the up limit
        private List<CommunicationAction> SetDownLimit = new List<CommunicationAction> { _setLimit_Down, _getMotorLimits };  //Sets the down limit
        private List<CommunicationAction> SetMotorData = new List<CommunicationAction> { _setMotorLabel, _getSingleMotorLabel, _setSpeeds, _getSpeeds, _setLimit_Down, _setLimit_Up, _getMotorLimits, _setMotorOrientation, _getMotorOrientation};
        private List<CommunicationAction> SetMotorGroupsAndIPs = new List<CommunicationAction> { _setGroupID, _setInternalPosition };
        private List<CommunicationAction> SetMotorName = new List<CommunicationAction> { _setMotorLabel, _getSingleMotorLabel };
        private List<CommunicationAction> SetSpeeds = new List<CommunicationAction> { _setSpeeds, _getSpeeds };
        private List<CommunicationAction> SetMotorDirection = new List<CommunicationAction> { _setMotorOrientation, _getMotorOrientation };
        private List<CommunicationAction> SetIPToCurrentPosition = new List<CommunicationAction> { _setIPToCurrentPosition, _getCurrentPositionIP };

        private List<CommunicationAction> SetKeypadAddress = new List<CommunicationAction> { _setKeypadAddress };
        private List<CommunicationAction> SearchForKeypads = new List<CommunicationAction> { _searchForKeypads, _validateKeypads, _getAllKeypadNames};
        private List<CommunicationAction> ProgramKeypadCommands = new List<CommunicationAction> { _programKeypadCommand, /*_programKeypadGroup*/ };
        private List<CommunicationAction> GetKeypadData = new List<CommunicationAction> { _startRetrievingKeypadCommand_Part4, _getKeypadCommand, /*_getKeypadGroupAddresses*/};
        private List<CommunicationAction> SetKeypadName = new List<CommunicationAction> { _setKeypadName, _getSingleKeypadName };

        private List<CommunicationAction> SetMACAddress = new List<CommunicationAction> { /*_startSettingMAC,*/ _setMACAddress };
        private List<CommunicationAction> SetIPAddress = new List<CommunicationAction> { /*_prepareIPAddress,*/ _setIPAddress };
        private List<CommunicationAction> RetrieveIPAddress = new List<CommunicationAction> { /*_prepareIPAddress,*/ _retrieveIPAddress };
        private List<CommunicationAction> SearchForAllDevices = new List<CommunicationAction> { _searchForKeypads, _validateKeypads, _getAllKeypadNames, _searchForMotors_Part3, _validateMotors, _getAllMotorLabels };
        private List<CommunicationAction> SetServerIPList = new List<CommunicationAction> { _sendIPListEntry };

        Dictionary<Messages, List<CommunicationAction>> _dictionaryOfMessages = new Dictionary<Messages, List<CommunicationAction>>();  //Setting up the dictionary that will hold all the messages
        #endregion

        #region Constructor
        public MessageScripts()
        {
            //Add all messages to the dictionary
            _dictionaryOfMessages.Add(Messages.SEARCH_FOR_MOTORS, SearchForMotors);
            _dictionaryOfMessages.Add(Messages.SEARCH_FOR_MOTORS_GROUPS_AND_IPS, SearchForMotorsGroupsAndIPs);
            _dictionaryOfMessages.Add(Messages.UPDATE_MOTOR_PAGE, GetMotorData);
            _dictionaryOfMessages.Add(Messages.UPDATE_GROUPS_AND_IPS, GetGroupsAndIPs);
            _dictionaryOfMessages.Add(Messages.MOVE_MOTOR_UP, MoveMotorUp);
            _dictionaryOfMessages.Add(Messages.MOVE_MOTOR_DOWN, MoveMotorDown);
            _dictionaryOfMessages.Add(Messages.STOP_MOTOR_MOVEMENT, StopMotorMovement);
            _dictionaryOfMessages.Add(Messages.START_IDENTIFICATION_MOVEMENT, StartIDMovement);
            _dictionaryOfMessages.Add(Messages.MOVE_TO_POSITION, MoveToPosition);
            _dictionaryOfMessages.Add(Messages.MOVE_TO_UP_LIMIT, MoveToUpLimit);
            _dictionaryOfMessages.Add(Messages.MOVE_TO_DOWN_LIMIT, MoveToDownLimit);
            _dictionaryOfMessages.Add(Messages.MOVE_TO_SELECTED_IP, MoveToSelectedIP);
            _dictionaryOfMessages.Add(Messages.FACTORY_RESET_MOTOR, FactoryResetMotor);
            _dictionaryOfMessages.Add(Messages.SET_MOTOR_ADDRESS, SetNewFAMotorAddress);
            _dictionaryOfMessages.Add(Messages.SET_UP_LIMIT, SetUpLimit);
            _dictionaryOfMessages.Add(Messages.SET_DOWN_LIMIT, SetDownLimit);
            _dictionaryOfMessages.Add(Messages.GET_MOTOR_DATA, GetMotorData);
            _dictionaryOfMessages.Add(Messages.SAVE_NEW_MOTOR_SETTINGS, SetMotorData);
            _dictionaryOfMessages.Add(Messages.SAVE_GROUPS_AND_IPS, SetMotorGroupsAndIPs);
            _dictionaryOfMessages.Add(Messages.SET_MOTOR_NAME, SetMotorName);
            _dictionaryOfMessages.Add(Messages.SET_SPEEDS, SetSpeeds);
            _dictionaryOfMessages.Add(Messages.SET_MOVEMENT_DIRECTION, SetMotorDirection);
            _dictionaryOfMessages.Add(Messages.SET_IP_TO_CURRENT_POSITION, SetIPToCurrentPosition);

            _dictionaryOfMessages.Add(Messages.SEARCH_FOR_KEYPADS, SearchForKeypads);
            _dictionaryOfMessages.Add(Messages.SAVE_NEW_KEYPAD_ADDRESS, SetKeypadAddress);
            _dictionaryOfMessages.Add(Messages.PROGRAM_KEYPAD_COMMANDS, ProgramKeypadCommands);
            _dictionaryOfMessages.Add(Messages.GET_BUTTON_DATA_FROM_KEYPAD, GetKeypadData);
            _dictionaryOfMessages.Add(Messages.SET_KEYPAD_NAME, SetKeypadName);

            _dictionaryOfMessages.Add(Messages.SET_MAC_ADDRESS, SetMACAddress);
            _dictionaryOfMessages.Add(Messages.SET_IP_ADDRESS, SetIPAddress);
            _dictionaryOfMessages.Add(Messages.GET_IP_ADDRESS, RetrieveIPAddress);
            _dictionaryOfMessages.Add(Messages.SEARCH_FOR_ALL_DEVICES, SearchForAllDevices);
            _dictionaryOfMessages.Add(Messages.SET_SERVER_IPS, SetServerIPList);
        }
        #endregion

        #region Accessors
        public Dictionary<Messages, List<CommunicationAction>> DictionaryOfMessages { get { return _dictionaryOfMessages; } }
        #endregion
    }
}
