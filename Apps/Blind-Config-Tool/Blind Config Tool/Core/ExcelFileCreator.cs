using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using OfficeOpenXml;
using OfficeOpenXml.Table;
using OfficeOpenXml.DataValidation;
using System.Windows.Input;

namespace Blind_Config_Tool.Core
{
    public static class ExcelFileCreator
    {
        public static void SaveToExcelFile(ObservableCollection<KeypadCommand> commandList, string filePath)
        {
            // Create a new Excel application object
            try
            {
                //string currentDirectory = Directory.GetCurrentDirectory();
                //string filePath = Path.Combine(currentDirectory, "Keypad Commands.xlsx");

                filePath = filePath + "\\Keypad Commands.xlsx";
                using (ExcelPackage excelPackage = new ExcelPackage(new FileInfo(filePath)))
                {
                    ExcelWorksheet worksheet = excelPackage.Workbook.Worksheets.FirstOrDefault(ws => ws.Name == "Keypad Commands");

                    if(worksheet != null)
                    {
                        worksheet.Cells.Clear();
                    }
                    else
                    {
                        worksheet = excelPackage.Workbook.Worksheets.Add("Keypad Commands");
                    }

                    worksheet.Cells[1, 1].Value = "On Press Action";
                    worksheet.Cells[1, 2].Value = "On Press Value";
                    worksheet.Cells[1, 3].Value = "On Hold Action";
                    worksheet.Cells[1, 4].Value = "On Hold Value";
                    worksheet.Cells[1, 5].Value = "On Release Action";
                    worksheet.Cells[1, 6].Value = "On Release Value";
                    worksheet.Cells[1, 7].Value = "Target";
                    worksheet.Cells[1, 8].Value = "Target Address";

                    for(int i = 0; i < commandList.Count; i++)
                    {
                        worksheet.Cells[i + 2, 1].Value = commandList[i].OnPressAction;
                        worksheet.Cells[i + 2, 2].Value = int.Parse(commandList[i].OnPressTarget);
                        var validation = worksheet.Cells[i + 2, 1].DataValidation.AddListDataValidation();
                        foreach(KeypadButtonAction action in Enum.GetValues(typeof(KeypadButtonAction)))
                        {
                            validation.Formula.Values.Add(action.ToString());
                        }
                        validation.AllowBlank = false;
                        validation.ShowErrorMessage = true;
                        validation.ErrorStyle = ExcelDataValidationWarningStyle.stop;
                        validation.ErrorTitle = "Invalid Value";
                        validation.Error = "Invalid value. Please select a value from the dropdown list.";

                        worksheet.Cells[i + 2, 3].Value = commandList[i].OnHoldAction;
                        worksheet.Cells[i + 2, 4].Value = int.Parse(commandList[i].OnHoldTarget);
                        validation = worksheet.Cells[i + 2, 3].DataValidation.AddListDataValidation();
                        foreach (KeypadButtonAction action in Enum.GetValues(typeof(KeypadButtonAction)))
                        {
                            validation.Formula.Values.Add(action.ToString());
                        }
                        validation.AllowBlank = false;
                        validation.ShowErrorMessage = true;
                        validation.ErrorStyle = ExcelDataValidationWarningStyle.stop;
                        validation.ErrorTitle = "Invalid Value";
                        validation.Error = "Invalid value. Please select a value from the dropdown list.";

                        worksheet.Cells[i + 2, 5].Value = commandList[i].OnReleaseAction;
                        worksheet.Cells[i + 2, 6].Value = int.Parse(commandList[i].OnReleaseTarget);
                        validation = worksheet.Cells[i + 2, 5].DataValidation.AddListDataValidation();
                        foreach (KeypadButtonAction action in Enum.GetValues(typeof(KeypadButtonAction)))
                        {
                            validation.Formula.Values.Add(action.ToString());
                        }
                        validation.AllowBlank = false;
                        validation.ShowErrorMessage = true;
                        validation.ErrorStyle = ExcelDataValidationWarningStyle.stop;
                        validation.ErrorTitle = "Invalid Value";
                        validation.Error = "Invalid value. Please select a value from the dropdown list.";

                        worksheet.Cells[i + 2, 7].Value = commandList[i].TargetMotor;
                        validation = worksheet.Cells[i + 2, 7].DataValidation.AddListDataValidation();
                        foreach (TargetMotor motor in Enum.GetValues(typeof(TargetMotor)))
                        {
                            validation.Formula.Values.Add(motor.ToString());
                        }
                        validation.AllowBlank = false;
                        validation.ShowErrorMessage = true;
                        validation.ErrorStyle = ExcelDataValidationWarningStyle.stop;
                        validation.ErrorTitle = "Invalid Value";
                        validation.Error = "Invalid value. Please select a value from the dropdown list.";

                        worksheet.Cells[i + 2, 8].Value = commandList[i].TargetAddress;
                    }

                    worksheet.Cells.AutoFitColumns();

                    excelPackage.SaveAs(new System.IO.FileInfo(filePath));

                    MessageBox.Show("All keypad commands saved successfully to " + filePath +".", "Commands Successfully Exported");
                }
            }
            catch (Exception ex)
            {
                ErrorMessages.DisplayError(ErrorCode.EXCEL_SAVE_TO_FILE_ERROR, ex.Message);
            }
        }

        public static List<KeypadCommand> LoadFromExcelFile(string filename)
        {
            List<KeypadCommand> commands = new List<KeypadCommand>();

            try
            {
                using (var package = new ExcelPackage(filename))
                {
                    // Get the first worksheet in the Excel file
                    ExcelWorksheet worksheet = package.Workbook.Worksheets[0];

                    // Read the data from the worksheet
                    int rowCount = worksheet.Dimension.Rows;
                    int colCount = worksheet.Dimension.Columns;

                    string validation = ValidateExcelFileDimensions(worksheet);

                    if (validation != "")
                    {
                        throw new Exception(validation);
                    }
                    else
                    {
                        for (int row = 2; row <= rowCount; row++)
                        {
                            commands.Add(new KeypadCommand(row - 2));

                            KeypadButtonAction action;
                            Enum.TryParse(worksheet.Cells[row, 1].Value.ToString(), out action);
                            int target = ValidateKeypadActionTarget(action, worksheet.Cells[row, 2].Value.ToString());
                            commands[row - 2].OnPressAction = action;

                            if (target <= -1)
                            {
                                throw new Exception("Invalid content error | row " + row + ", column 2");
                            }
                            else
                            {
                                commands[row - 2].OnPressTarget = target.ToString();
                            }

                            Enum.TryParse(worksheet.Cells[row, 3].Value.ToString(), out action);
                            target = ValidateKeypadActionTarget(action, worksheet.Cells[row, 4].Value.ToString());
                            commands[row - 2].OnHoldAction = action;

                            if (target <= -1)
                            {
                                throw new Exception("Invalid content error | row " + row + ", column 4");
                            }
                            else
                            {
                                commands[row - 2].OnHoldTarget = target.ToString();
                            }

                            Enum.TryParse(worksheet.Cells[row, 5].Value.ToString(), out action);
                            target = ValidateKeypadActionTarget(action, worksheet.Cells[row, 6].Value.ToString());
                            commands[row - 2].OnReleaseAction = action;

                            if (target <= -1)
                            {
                                throw new Exception("Invalid content error | row " + row + ", column 6");
                            }
                            else
                            {
                                commands[row - 2].OnReleaseTarget = target.ToString();
                            }
                            

                            TargetMotor targetMotor;
                            Enum.TryParse(worksheet.Cells[row, 7].Value.ToString(), out targetMotor);
                            commands[row - 2].TargetMotor = targetMotor;

                            string obj = worksheet.Cells[row, 8].Value.ToString();
                            if (int.TryParse(worksheet.Cells[row, 8].Value.ToString(), System.Globalization.NumberStyles.HexNumber, null, out int addr) == false || worksheet.Cells[row, 8].Value.ToString().Length != 6)
                            {
                                throw new Exception("Invalid content error | row " + row + ", column 8");
                            }
                            else
                            {
                                commands[row - 2].TargetAddress = worksheet.Cells[row, 8].Value.ToString().ToUpper();
                            }

                            commands[row - 2].Changed = true;
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                ErrorMessages.DisplayError(ErrorCode.EXCEL_LOAD_FROM_FILE_ERROR, ex.Message);
                commands = new List<KeypadCommand>();
            }

            return commands;
        }

        private static string ValidateExcelFileDimensions(ExcelWorksheet sheet)
        {
            string retVal = "";

            if(sheet.Dimension.Columns != 8)
            {
                retVal = "Incorrect number of columns";
            }
            else
            {
                if(sheet.Dimension.Rows <= 1)
                {
                    retVal = "Incorrect number of rows";
                }
            }

            return retVal;
        }

        private static int ValidateKeypadActionTarget(KeypadButtonAction action, string target)
        {
            if (int.TryParse(target, out int result) == false)
            {
                result = -1;
            }
            else
            {
                int minValue = GetMinValue(action);
                int maxValue = GetMaxValue(action);

                if (result < minValue)
                {
                    result = minValue;
                }
                else if (result > maxValue)
                {
                    result = maxValue;
                }
            }

            return result;
        }

        private static int GetMinValue(KeypadButtonAction action)
        {
            int retVal = 0;

            switch (action)
            {
                case KeypadButtonAction.GO_TO_IP:
                    retVal = 1;
                    break;
            }

            return retVal;
        }

        private static int GetMaxValue(KeypadButtonAction action)
        {
            int maxValue = 0;

            switch (action)
            {
                case KeypadButtonAction.GO_TO_IP:
                    maxValue = 16;
                    break;
                case KeypadButtonAction.JOG_UP_MM:
                    maxValue = 250;
                    break;
                case KeypadButtonAction.JOG_DOWN_MM:
                    maxValue = 250;
                    break;
                case KeypadButtonAction.JOG_UP_PULSE:
                    maxValue = 999;
                    break;
                case KeypadButtonAction.JOG_DOWN_PULSE:
                    maxValue = 999;
                    break;
                case KeypadButtonAction.GO_TO_PERCENT:
                    maxValue = 100;
                    break;
            }

            return maxValue;
        }
    }
}
