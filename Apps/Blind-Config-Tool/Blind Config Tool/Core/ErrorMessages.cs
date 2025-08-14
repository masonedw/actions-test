using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.IO;
using System.Security.Cryptography.Xml;
using System.Windows.Forms;

namespace Blind_Config_Tool.Core
{
    public enum ErrorCode
    {
        SERIAL_NOT_OPEN = 1000,
        SERIAL_CANNOT_CONNECT = 1001,
        SERIAL_IN_USE = 1002,
        SERIAL_ALREADY_OPEN = 1003,
        SERIAL_CANNOT_CLOSE = 1004,
        SERIAL_NO_PORT = 1005,
        NO_MOTORS_FOUND = 1006,

        INVALID_SPEEDS = 1100,
        INVALID_MOVE_POSITION = 1101,
        COULD_NOT_SET_LIMIT = 1102,
        CHANGED_TO_INVALID_MOTOR = 1103,
        NO_LIMITS_SET = 1104,
        FAILED_GET_SPEEDS = 1105,
        FAILED_GET_LIMITS = 1106,
        FAILED_GET_ORIENTATION = 1107,
        COULD_NOT_SET_SPEEDS = 1108,
        INVALID_INTERNAL_POSITION = 1009,
        NOT_FA_MOTOR = 1010,

        NO_KEYPAD_FOUND = 1200,
        SET_KEYPAD_ADDRESS_FAILED = 1201,
        COULD_NOT_PROGRAM_COMMAND = 1202,
        COULD_NOT_RETRIEVE_KEYPAD_COMMAND = 1203,
        NO_KEYPAD_COMMAND_SELECTED = 1204,
        COULD_NOT_RETRIEVE_GROUP_ADDRESSES = 1205,
        NO_KEYPAD_SELECTED = 1206,

        EXCEL_SAVE_TO_FILE_ERROR = 1300,
        EXCEL_LOAD_FROM_FILE_ERROR = 1301,
        
        UPDATE_LOG_FILE_ERROR = 1400
    }

    public static class ErrorMessages
    {
        static int maxLogLines = 1000;  //Max lines of text the log file can have before being overwritten
        static string logFilePath = Directory.GetCurrentDirectory() + "\\Log.txt";  //The path to the log file

        static readonly Error _Serial_NotOpen = new Error(ErrorCode.SERIAL_NOT_OPEN, "Serial Port Not Open", "There is no open serial port. Please open a serial port and then try again.");
        static readonly Error _Serial_CannotConnect = new Error(ErrorCode.SERIAL_CANNOT_CONNECT, "Cannot Connect To Serial", "A connection to the desired serial port could not be established. Please check that the chosen serial port is correct and not in use by another process.");
        static readonly Error _Serial_PortInUse = new Error(ErrorCode.SERIAL_IN_USE, "Serial Port In Use", "This serial port is already being used by another process. Please choose another serial port or close any applications that are using this port.");
        static readonly Error _Serial_PortAlreadyOpen = new Error(ErrorCode.SERIAL_ALREADY_OPEN, "Serial Port Already Open", "This serial port is already open and connected to this process.");
        static readonly Error _Serial_CannotClose = new Error(ErrorCode.SERIAL_CANNOT_CLOSE, "Cannot Close Port", "Cannot close the serial port. Please check that the port is open and try again.");
        static readonly Error _Serial_NoPort = new Error(ErrorCode.SERIAL_NO_PORT, "No Port Found", "Could not find a valid serial port. Please check the connection and try again.");
        static readonly Error _Serial_NoMotorsFound = new Error(ErrorCode.NO_MOTORS_FOUND, "No Motors Found", "No valid motors could be found. Please check all cable connections and try again.");

        static readonly Error _MotorViewModel_InvalidSpeeds = new Error(ErrorCode.INVALID_SPEEDS, "Invalid Speeds", "Invalid speeds. Both speed values must be greater than 1.");
        static readonly Error _MotorViewModel_InvalidMovePosition = new Error(ErrorCode.INVALID_MOVE_POSITION, "Invalid Move Position", "Please enter a movement value between 0 and 100, then try again.");
        static readonly Error _MotorViewModel_SetLimitInvalid = new Error(ErrorCode.COULD_NOT_SET_LIMIT, "Could Not Set Limit", "Could not set the limit. It is possible that the motor is in the wrong position. Please move the motor up or down and try again.");
        static readonly Error _MotorViewModel_Changed_To_Invalid_Motor = new Error(ErrorCode.CHANGED_TO_INVALID_MOTOR, "Changed To Invalid Motor", "The system is trying to display data from an invalid motor. Please restart the application and try again.");
        static readonly Error _MotorViewModel_No_Limits_Set = new Error(ErrorCode.NO_LIMITS_SET, "No Limits Set", "The end limits have not been set. Please set both limits using the buttons provided and then try again.");
        static readonly Error _MotorViewModel_Failed_Get_Speeds = new Error(ErrorCode.FAILED_GET_SPEEDS, "Get Speeds Failure", "Could not get the motor speeds. Please try again.");
        static readonly Error _MotorViewModel_Failed_Get_Limits = new Error(ErrorCode.FAILED_GET_LIMITS, "Get limits Failure", "Could not get the motor limits. Please try again.");
        static readonly Error _MotorViewModel_Failed_Get_Orientation = new Error(ErrorCode.FAILED_GET_ORIENTATION, "Get orientation Failure", "Could not get the motor orientation. Please try again.");
        static readonly Error _MotorViewModel_SetSpeedsInvalid = new Error(ErrorCode.COULD_NOT_SET_SPEEDS, "Could Not Set Speeds", "Could not set the motor speeds. The speeds may be invalid or the motor may not have adjustable speed settings.");
        static readonly Error _MotorViewModel_InvalidInternalPosition = new Error(ErrorCode.INVALID_INTERNAL_POSITION, "Invalid Internal Position", "Internal positions cannot be less than 1 or greater than 99. Internal position {%} could not be set.");
        static readonly Error _MotorViewModel_NotAnFAMotor = new Error(ErrorCode.NOT_FA_MOTOR, "Motor Address Not Changed", "The chosen motor is not a Future Automation motor. Only Future Automation motors can be changed.");

        static readonly Error _keypadViewModel_NoKeypadFound = new Error(ErrorCode.NO_KEYPAD_FOUND, "No Keypad Found", "No keypad could be found on the system. Please check the connection to the keypad and try again.");
        static readonly Error _keypadViewModel_SetKeypadAddressFailed = new Error(ErrorCode.SET_KEYPAD_ADDRESS_FAILED, "Set Keypad Address Failure", "The keypad address could not be set. Please check you are using a valid address and try again.");
        static readonly Error _keypadViewModel_CouldNotProgramCommand = new Error(ErrorCode.COULD_NOT_PROGRAM_COMMAND, "Program Command {%} Failure", "Failed to program command {%}. Please check the command and try again.");
        static readonly Error _keypadViewModel_CouldNotRetrieveKeypadCommand = new Error(ErrorCode.COULD_NOT_RETRIEVE_KEYPAD_COMMAND, "Retrieve Command {%} Failure", "Failed to retrieve command {%}. Please try again.");
        static readonly Error _keypadViewModel_NoKeypadCommandSelected = new Error(ErrorCode.NO_KEYPAD_COMMAND_SELECTED, "No Keypad Command Selected", "No keypad command selected. Please select a command from the list before attempting to save any command settings.");
        static readonly Error _keypadViewModel_CouldNotRetrieveGroups = new Error(ErrorCode.COULD_NOT_RETRIEVE_GROUP_ADDRESSES, "Retrieve Groups Failure", "Failed to retrieve the command groups. Please try again");
        static readonly Error _keypadViewModel_NoValidKeypadSelected = new Error(ErrorCode.NO_KEYPAD_SELECTED, "No Keypad Selected", "No valid keypad selected. Please search for keypads using the 'Search For Keypads' button, then select the desired keypad and try again.");

        static readonly Error _ExcelFileCreator_SaveToFileError = new Error(ErrorCode.EXCEL_SAVE_TO_FILE_ERROR, "Excel Save To File Error", "Error occurred. Could not save to file.\n\nError: {%}");
        static readonly Error _ExcelFileCreator_LoadFromFileError = new Error(ErrorCode.EXCEL_LOAD_FROM_FILE_ERROR, "Excel Load From File Error", "Error occurred. Could not load from file. \n\nError: {%}");

        static readonly Error _LogFile_UpdateLogError = new Error(ErrorCode.UPDATE_LOG_FILE_ERROR, "Update Log Error", "An error occurred when updating the log file. Error: {%}");
        
        static readonly Dictionary<ErrorCode, Error> _dictionaryOfErrors = new Dictionary<ErrorCode, Error>();  //Setting up the dictionary that will hold all the errors

        #region Constructor
        static ErrorMessages()
        {
            //Add all messages to the dictionary
            _dictionaryOfErrors.Add(_Serial_NotOpen.ErrorCode, _Serial_NotOpen);
            _dictionaryOfErrors.Add(_Serial_CannotConnect.ErrorCode, _Serial_CannotConnect);
            _dictionaryOfErrors.Add(_Serial_PortInUse.ErrorCode, _Serial_PortInUse);
            _dictionaryOfErrors.Add(_Serial_PortAlreadyOpen.ErrorCode, _Serial_PortAlreadyOpen);
            _dictionaryOfErrors.Add(_Serial_CannotClose.ErrorCode, _Serial_CannotClose);
            _dictionaryOfErrors.Add(_Serial_NoPort.ErrorCode, _Serial_NoPort);
            _dictionaryOfErrors.Add(_Serial_NoMotorsFound.ErrorCode, _Serial_NoMotorsFound);

            _dictionaryOfErrors.Add(_MotorViewModel_InvalidSpeeds.ErrorCode, _MotorViewModel_InvalidSpeeds);
            _dictionaryOfErrors.Add(_MotorViewModel_InvalidMovePosition.ErrorCode, _MotorViewModel_InvalidMovePosition);
            _dictionaryOfErrors.Add(_MotorViewModel_SetLimitInvalid.ErrorCode, _MotorViewModel_SetLimitInvalid);
            _dictionaryOfErrors.Add(_MotorViewModel_Changed_To_Invalid_Motor.ErrorCode, _MotorViewModel_Changed_To_Invalid_Motor);
            _dictionaryOfErrors.Add(_MotorViewModel_No_Limits_Set.ErrorCode, _MotorViewModel_No_Limits_Set);
            _dictionaryOfErrors.Add(_MotorViewModel_Failed_Get_Speeds.ErrorCode, _MotorViewModel_Failed_Get_Speeds);
            _dictionaryOfErrors.Add(_MotorViewModel_Failed_Get_Limits.ErrorCode, _MotorViewModel_Failed_Get_Limits);
            _dictionaryOfErrors.Add(_MotorViewModel_Failed_Get_Orientation.ErrorCode, _MotorViewModel_Failed_Get_Orientation);
            _dictionaryOfErrors.Add(_MotorViewModel_SetSpeedsInvalid.ErrorCode, _MotorViewModel_SetSpeedsInvalid);
            _dictionaryOfErrors.Add(_MotorViewModel_InvalidInternalPosition.ErrorCode, _MotorViewModel_InvalidInternalPosition);
            _dictionaryOfErrors.Add(_MotorViewModel_NotAnFAMotor.ErrorCode, _MotorViewModel_NotAnFAMotor);

            _dictionaryOfErrors.Add(_keypadViewModel_NoKeypadFound.ErrorCode, _keypadViewModel_NoKeypadFound);
            _dictionaryOfErrors.Add(_keypadViewModel_SetKeypadAddressFailed.ErrorCode, _keypadViewModel_SetKeypadAddressFailed);
            _dictionaryOfErrors.Add(_keypadViewModel_CouldNotProgramCommand.ErrorCode, _keypadViewModel_CouldNotProgramCommand);
            _dictionaryOfErrors.Add(_keypadViewModel_CouldNotRetrieveKeypadCommand.ErrorCode, _keypadViewModel_CouldNotRetrieveKeypadCommand);
            _dictionaryOfErrors.Add(_keypadViewModel_NoKeypadCommandSelected.ErrorCode, _keypadViewModel_NoKeypadCommandSelected);
            _dictionaryOfErrors.Add(_keypadViewModel_CouldNotRetrieveGroups.ErrorCode, _keypadViewModel_CouldNotRetrieveGroups);
            _dictionaryOfErrors.Add(_keypadViewModel_NoValidKeypadSelected.ErrorCode, _keypadViewModel_NoValidKeypadSelected);

            _dictionaryOfErrors.Add(_ExcelFileCreator_SaveToFileError.ErrorCode, _ExcelFileCreator_SaveToFileError);
            _dictionaryOfErrors.Add(_ExcelFileCreator_LoadFromFileError.ErrorCode, _ExcelFileCreator_LoadFromFileError);

            _dictionaryOfErrors.Add(_LogFile_UpdateLogError.ErrorCode, _LogFile_UpdateLogError);

            if (File.Exists(logFilePath) == true)   //If a log file in the correct location already 
            {
                File.Delete(logFilePath);   //Delete the log file
            }

            FileStream fs = File.Create(LogFilePath);   //Create a log file in the correct location
            fs.Close(); //Close the file
        }
        #endregion

        public static void DisplayError(ErrorCode code)
        {
            Error err = _dictionaryOfErrors[code];  //Find the error information in the dictionary

            System.Windows.MessageBox.Show(err.ErrorCodeDescription, (err.ErrorTitle + " (#" + (int)err.ErrorCode) + ")");  //Display the corresponding error

            //UpdateLogFile(err.ErrorCode.ToString() + "(#" + (int)err.ErrorCode + ") - " + err.ErrorTitle);    //Update the log file
        }

        public static void DisplayError(ErrorCode code, string replacement) 
        {
            Error err = _dictionaryOfErrors[code];  //Find the error information in the directory

            System.Windows.MessageBox.Show(err.ErrorCodeDescription.Replace("{%}", replacement), (err.ErrorTitle.Replace("{%}", replacement) + " (#" + (int)err.ErrorCode) + ")");  //Display the corresponding error

            //UpdateLogFile(err.ErrorCode.ToString() + "(#" + (int)err.ErrorCode + ") - " + err.ErrorTitle);    //Update the log file
        }

        public static void UpdateLogFile(string update)
        {
            try
            {
                if (File.ReadAllLines(logFilePath).Length >= maxLogLines)   //If the log file is too big
                {
                    File.WriteAllText(logFilePath, string.Empty);   //Empty the log file
                }

                using (StreamWriter writer = File.AppendText(logFilePath))  //Append text to the log file
                {
                    writer.WriteLine(System.DateTime.Now.ToString() + " | " + update);  //Append the desired information
                }
            }
            catch
            {
            }
        }

        public static void CopyLogFile()
        {
            // Create a new instance of SaveFileDialog
            SaveFileDialog saveFileDialog = new SaveFileDialog();

            // Set the default file name and extension
            saveFileDialog.FileName = "UsageReport.txt";
            saveFileDialog.DefaultExt = "txt";

            // Set the filter for the file extension and description
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

            // Display the SaveFileDialog
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Get the selected file name and location
                string filePath = saveFileDialog.FileName;

                try
                {
                    // Copy the file to the selected location
                    File.Copy(logFilePath, filePath);
                    System.Windows.MessageBox.Show("File saved successfully.");
                }
                catch (Exception ex)
                {
                    System.Windows.MessageBox.Show("Error saving file: " + ex.Message);
                }
            }
        }

        public static string LogFilePath { get { return logFilePath; } }
    }

    class Error
    {
        private ErrorCode _errorCode;
        private string _errorTitle;
        private string _errorDescription;

        public Error(ErrorCode code, string title, string description) 
        {   
            _errorCode= code;
            _errorTitle= title;
            _errorDescription= description;
        }

        public ErrorCode ErrorCode { get { return _errorCode;} }
        public string ErrorTitle { get { return _errorTitle;} }
        public string ErrorCodeDescription { get { return _errorDescription;} }
    }
}
