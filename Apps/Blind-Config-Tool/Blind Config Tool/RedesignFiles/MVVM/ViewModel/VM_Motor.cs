using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;

namespace Blind_Config_Tool.RedesignFiles.MVVM.ViewModel
{
    internal class VM_Motor : ObservableObject
    {
        //ContentControls
        object _currentView;
        VM_MotPage1 _page1 = new VM_MotPage1();
        VM_MotPage2 _page2 = new VM_MotPage2();
        bool onPage1 = true;

        AppData _appData;
        ScriptHandler _scriptHandler;

        ObservableCollection<Motor> _motorList = new ObservableCollection<Motor>();
        int _selectedMotor = -1;
        int _lastSelectedMotor = -1;

        bool _canClearMotors = false;
        Visibility _showListOverlay = Visibility.Visible;
        ObservableCollection<string> _sortMethods = new ObservableCollection<string>() { "Name (A → Z)", "Name (Z → A)", "Address (Ascending)", "Address (Descending)" };

        public VM_Motor()
        {
            _appData = AppData.Instance;
            _scriptHandler = ScriptHandler.Instance;            
            CurrentView = _page1;
            VM_MotPage1.OnMotorPageChange += ChangePage;
            VM_MotPage2.OnMotorPageChange += ChangePage;
            _appData.OnMotorListUpdated += UpdateMotorList; //Called whenever a new motor is added to the list of available motors
            _scriptHandler.OnToggleCanClearState += SetClearMotorEnable;
        }

        void ChangePage()
        {
            if(onPage1)
            {
                CurrentView = _page2;
                onPage1 = false;

                if(SelectedMotor >= 0)
                {
                    _scriptHandler.StartScript(Messages.UPDATE_GROUPS_AND_IPS);
                }
            }
            else
            {
                CurrentView = _page1;
                onPage1 = true;

                if (SelectedMotor >= 0)
                {
                    _scriptHandler.StartScript(Messages.UPDATE_MOTOR_PAGE);
                }
            }
        }

        private RelayCommand SearchForMotors1;
        public ICommand SearchForMotors
        {
            get
            {
                if (SearchForMotors1 == null)
                {
                    SearchForMotors1 = new RelayCommand(o =>
                    {
                        if (onPage1)
                        {
                            if (_appData.ResetValidationOnSearch)
                            {
                                foreach(Motor mot in _appData.Motors)
                                {
                                    mot.Validated = false;
                                }
                            }

                            _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, _appData.ValidationCheckTime);
                            _scriptHandler.StartScript(Messages.SEARCH_FOR_MOTORS);
                        }
                        else
                        {
                            _scriptHandler.StartScript(Messages.SEARCH_FOR_MOTORS_GROUPS_AND_IPS);
                        }
                    });
                }

                return SearchForMotors1;
            }
        }

        void UpdateMotorList()
        {
            MotorList = new ObservableCollection<Motor>(_appData.Motors);

            if (SelectedMotor < 0)
            {
                SelectedMotor = 0;
            }
        }

        void SetClearMotorEnable(bool toggle)
        {
            CanClearMotors = toggle;
        }

        private RelayCommand ClearList1;
        public ICommand ClearList
        {
            get
            {
                if (ClearList1 == null)
                {
                    ClearList1 = new RelayCommand(o =>
                    {
                        SelectedMotor = -1;
                        _appData.ClearMotorList();
                    });
                }

                return ClearList1;
            }
        }

        public object CurrentView { get { return _currentView; } set { _currentView = value; OnPropertyChanged(); } }
        public ObservableCollection<Motor> MotorList { get { return _motorList; } set { _motorList = value; OnPropertyChanged();} }
        public int SelectedMotor
        {
            get { return _selectedMotor; }
            set
            {
                _selectedMotor = value;
                _appData.MotorIndex = value;

                if (_selectedMotor != _lastSelectedMotor && _selectedMotor >= 0)
                {
                    if (_lastSelectedMotor >= 0 && _selectedMotor >= 0)
                    {
                        if (onPage1)
                        {
                            _scriptHandler.StartScript(Messages.UPDATE_MOTOR_PAGE);
                        }
                        else
                        {
                            _scriptHandler.StartScript(Messages.UPDATE_GROUPS_AND_IPS);
                        }
                        
                    }
                }

                if(_appData.Motors.Count <= 0)
                {
                    CanClearMotors = false;
                }

                _lastSelectedMotor = _selectedMotor;
                OnPropertyChanged();
            }
        }

        public bool CanClearMotors { get { return _canClearMotors; } 
            set 
            { 
                _canClearMotors = value;
                
                if(_canClearMotors)
                {
                    ShowListOverlay = Visibility.Hidden;
                }
                else
                {
                    ShowListOverlay = Visibility.Visible;
                }

                OnPropertyChanged(); 
            } 
        }
        public ObservableCollection<string> SortMethods { get { return _sortMethods; } set { _sortMethods = value; OnPropertyChanged(); } }
        public int SelectedSortType { get { return _appData.SelectedSortType; } set { _appData.SelectedSortType = value; OnPropertyChanged(); } }
        public Visibility ShowListOverlay { get { return _showListOverlay; } set { _showListOverlay = value; OnPropertyChanged(); } }
    }
}
