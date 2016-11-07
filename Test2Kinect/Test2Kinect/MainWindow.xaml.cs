using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Kinect;
using System.IO;
using System.Globalization;
using ArduinoDriver.SerialProtocol;


namespace Test2Kinect
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        //private static string portName = "COM3";
        //private static int baudRate = 9600;
        //System.IO.Ports.SerialPort Port = new System.IO.Ports.SerialPort(portName, baudRate);
        //private ArduinoDriver.ArduinoDriver AD = new ArduinoDriver.ArduinoDriver(portName, true);

        /// <summary>
        /// Active Kinect sensor
        /// </summary>
        private KinectSensor sensor;

        /// <summary>
        /// Bitmap that will hold color information
        /// </summary>
        private WriteableBitmap colorBitmap;

        private WriteableBitmap croppedBitmap;

        /// <summary>
        /// Intermediate storage for the color data received from the camera
        /// </summary>
        private byte[] colorPixels;

        /// <summary>
        /// Initializes a new instance of the MainWindow class.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
            //Port.DataBits = 8;
            //Port.Open();
            
        }
        
        

        
        /// <summary>
        /// Execute startup tasks
        /// </summary>
        /// <param name="sender">object sending the event</param>
        /// <param name="e">event arguments</param>
        private void WindowLoaded(object sender, RoutedEventArgs e)
        {


            // Look through all sensors and start the first connected one.
            // This requires that a Kinect is connected at the time of app startup.
            // To make your app robust against plug/unplug, 
            // it is recommended to use KinectSensorChooser provided in Microsoft.Kinect.Toolkit (See components in Toolkit Browser).
            foreach (var potentialSensor in KinectSensor.KinectSensors)
            {
                if (potentialSensor.Status == KinectStatus.Connected)
                {
                    this.sensor = potentialSensor;
                    break;
                }
            }

            if (null != this.sensor)
            {
                // Turn on the color stream to receive color frames
                this.sensor.ColorStream.Enable(ColorImageFormat.RgbResolution640x480Fps30);

                // Allocate space to put the pixels we'll receive
                this.colorPixels = new byte[this.sensor.ColorStream.FramePixelDataLength];

                // This is the bitmap we'll display on-screen
                this.colorBitmap = new WriteableBitmap(this.sensor.ColorStream.FrameWidth, this.sensor.ColorStream.FrameHeight, 96.0, 96.0, PixelFormats.Bgr32, null);

                // Set the image we display to point to the bitmap where we'll put the image data
                this.Image.Source = this.colorBitmap;

                // Add an event handler to be called whenever there is new color frame data
                this.sensor.ColorFrameReady += this.SensorColorFrameReady;

                // Start the sensor!
                try
                {
                    this.sensor.Start();
                }
                catch (IOException)
                {
                    this.sensor = null;
                }
            }

            if (null == this.sensor)
            {
                // this.statusBarText.Text = Properties.Resources.NoKinectReady;
            }

           

        }

        /// <summary>
        /// Execute shutdown tasks
        /// </summary>
        /// <param name="sender">object sending the event</param>
        /// <param name="e">event arguments</param>
        private void WindowClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (null != this.sensor)
            {
                this.sensor.Stop();
            }
            //Port.Close();
        }

        /// <summary>
        /// Event handler for Kinect sensor's ColorFrameReady event
        /// </summary>
        /// <param name="sender">object sending the event</param>
        /// <param name="e">event arguments</param>
        private void SensorColorFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            using (ColorImageFrame colorFrame = e.OpenColorImageFrame())
            {
                if (colorFrame != null)
                {
                    // Copy the pixel data from the image to a temporary array
                    colorFrame.CopyPixelDataTo(this.colorPixels);

                    Boolean Error = true;

                    // Write the pixel data into our bitmap
                    while (Error == true)
                    {
                        try
                        {
                            Error = false;
                            this.colorBitmap.WritePixels(
                            new Int32Rect(0, 0, this.colorBitmap.PixelWidth, this.colorBitmap.PixelHeight),
                            this.colorPixels,
                            this.colorBitmap.PixelWidth * sizeof(int),
                            0);
                        }
                        catch (System.Runtime.InteropServices.COMException)
                        {
                            Error = true;

                        }
                    }
                    
                    
                }
            }
        }

        private void SensorColorPictureFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            using (ColorImageFrame colorFrame2 = e.OpenColorImageFrame())
            {
                if (colorFrame2 != null)
                {
                    // Copy the pixel data from the image to a temporary array
                    colorFrame2.CopyPixelDataTo(this.colorPixels);

                    // Write the pixel data into our bitmap
                    this.colorBitmap.WritePixels(
                        new Int32Rect(0, 0, this.colorBitmap.PixelWidth, this.colorBitmap.PixelHeight),
                        this.colorPixels,
                        this.colorBitmap.PixelWidth * sizeof(int),
                        0);
                }
            }
        }

        /// <summary>
        /// Handles the user clicking on the screenshot button
        /// </summary>
        /// <param name="sender">object sending the event</param>
        /// <param name="e">event arguments</param>
        private void ButtonScreenshotClick(object sender, RoutedEventArgs e)
        {
            

                    if (null == this.sensor)
                    {
                        //this.statusBarText.Text = Properties.Resources.ConnectDeviceFirst;
                        return;
                    }

                    // create a png bitmap encoder which knows how to save a .png file
                    BitmapEncoder encoder = new PngBitmapEncoder();

            // create frame from the writable bitmap and add to encoder
            //encoder.Frames.Add(BitmapFrame.Create(this.colorBitmap));
                    croppedBitmap = Crop.CopyPixelsTo(this.colorBitmap, new Int32Rect(160, 100, this.colorBitmap.PixelWidth/2, this.colorBitmap.PixelHeight/2), new Int32Rect(0, 0, 200, 200));
                    encoder.Frames.Add(BitmapFrame.Create(croppedBitmap));

                    this.Image2.Source = encoder.Frames.ElementAt(encoder.Frames.Count - 1);
 
            return;
        }

                     


        /* private void ArduinoPicture(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
         {


                 if (null == this.sensor)
                 {
                     //this.statusBarText.Text = Properties.Resources.ConnectDeviceFirst;
                     return;
                 }

                 // create a png bitmap encoder which knows how to save a .png file
                 BitmapEncoder encoder = new PngBitmapEncoder();

                 // create frame from the writable bitmap and add to encoder
                 encoder.Frames.Add(BitmapFrame.Create(this.colorBitmap));
                 this.Image2.Source = encoder.Frames.ElementAt(encoder.Frames.Count - 1);



             return;

         }*/

    }
}
