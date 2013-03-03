using System.Diagnostics;
using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using DestroyAllHumans.ViewModel;


namespace DestroyAllHumans.View
{

    public partial class MainWindow : Window
    {
        private MainVM VM = new MainVM();
        private SerialPort usbCOM = new SerialPort();
        private SerialPort blueToothCOM = new SerialPort();

          private static byte COMMAND_KEY = 33;
          private static byte CMD_DRIVE_X_FWD = 65;
          private static byte CMD_DRIVE_X_BACK = 66;
          private static byte CMD_DRIVE_Y_FWD = 67;
          private static byte CMD_DRIVE_Y_BACK = 68;
          private static byte CMD_DRIVE_STEPMODE = 69;
          private static byte CMD_DRIVE_X_SPS = 70;
          private static byte CMD_DRIVE_Y_SPS = 71;

        private byte[] cmd = {COMMAND_KEY, 0x0, 0x0, 0x0};

        private byte stepAmount = 0xFF;

        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = VM;
        }


        private void begin_Click(object sender, RoutedEventArgs e)
        {
            //InitForm.Visibility = System.Windows.Visibility.Collapsed;
            //MainAppView.Visibility = System.Windows.Visibility.Visible;

            // USE VALUES FROM TEXT BOXES HERE

            if (usbEnable.IsChecked.Value)
            {
                 usbCOM.PortName = txt_USBCOM.Text;
                 usbCOM.Open();
                 if (!usbCOM.IsOpen)
                 {
                      VM.DebugInfo = "Device Connection Not Connected, Check Connectivity and Restart App";
                 }
            }

            if (btEnable.IsChecked.Value)
            {
                 blueToothCOM.PortName = txt_BTCOM.Text;
                 blueToothCOM.Open();
                 if (!blueToothCOM.IsOpen)
                 {
                      VM.DebugInfo = "Device Connection Not Connected, Check Connectivity and Restart App";
                 }
            }

        }

        private void leftButton_Click(object sender, RoutedEventArgs e)
        {
             if (usbCOM.IsOpen)
             {
                  cmd[1] = CMD_DRIVE_X_FWD;
                  cmd[2] = 0x0;
                  cmd[3] = stepAmount;
                  usbCOM.Write(cmd, 0, 4);
             }
        }

        private void rightButton_Click(object sender, RoutedEventArgs e)
        {
             if (usbCOM.IsOpen)
             {
                  cmd[1] = CMD_DRIVE_X_BACK;
                  cmd[2] = 0x0;
                  cmd[3] = stepAmount;
                  usbCOM.Write(cmd, 0, 4);
             }
        }

        private void upButton_Click(object sender, RoutedEventArgs e)
        {
             if (usbCOM.IsOpen)
             {
                  cmd[1] = CMD_DRIVE_Y_FWD;
                  cmd[2] = 0x0;
                  cmd[3] = stepAmount;
                  usbCOM.Write(cmd, 0, 4);
             }
        }

        private void downButton_Click(object sender, RoutedEventArgs e)
        {

             if (usbCOM.IsOpen)
             {
                  cmd[1] = CMD_DRIVE_Y_BACK;
                  cmd[2] = 0x0;
                  cmd[3] = stepAmount;
                  usbCOM.Write(cmd, 0, 4);
             }
        }

        private void speedSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
             if (usbCOM.IsOpen)
             {
                  cmd[1] = CMD_DRIVE_X_SPS;
                  cmd[2] = (byte) e.NewValue;
                  cmd[3] = 0x0;
                  usbCOM.Write(cmd, 0, 4);
             }
        }
    }
}
