using System;
using System.Timers;
using static Blind_Config_Tool.RedesignFiles.AppData;

namespace Blind_Config_Tool.RedesignFiles
{
    public class ActionTimer
    {
        private Timers _name;
        private Timer _timer;
        private Action _action;
        private int _interval;
        private bool _active;

        public ActionTimer(Timers name, Action action)
        {
            _name = name;
            _action = action;
            _timer = new Timer();
            _timer.Elapsed += OnElapsed;
            _timer.AutoReset = true; // Keep firing at interval
        }

        public void Start(int interval)
        {
            _interval = interval;
            _timer.Interval = interval;
            _timer.Start();
            _active = true;
        }

        public void Reset()
        {
            Stop();
            Start(_interval);
        }

        public void Stop()
        {
            _timer.Stop();
            _active = false;
        }

        private void OnElapsed(object sender, ElapsedEventArgs e)
        {
            _action?.Invoke();
        }

        public bool IsRunning => _active;
    }
}
