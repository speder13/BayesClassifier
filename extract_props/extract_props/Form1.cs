using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using Accord.Imaging;

namespace extract_props
{
    public partial class Form1 : Form
    {
        int counter = 0;
        int filesLoaded = 0;
        int[] properties;
        string[] emptyImages;
        string[] ballImages;
        
        Accord.Imaging.Converters.ImageToArray arrayMaker = new Accord.Imaging.Converters.ImageToArray();
        Accord.Imaging.Converters.ArrayToImage imageMaker = new Accord.Imaging.Converters.ArrayToImage(400,200);
        Dictionary<int, int> backgrounds = new Dictionary<int, int>();

        public Form1()
        {
            InitializeComponent();
            backgrounds.Add(0,0);
        }

        public void load_files()
        {
            emptyImages = Directory.GetFiles("images/Empty/","*.*");
            ballImages = Directory.GetFiles("images/Blue/", "*.*"); ;
            filesLoaded = ballImages.Count();
            progressBar3.Value = 100;
            labelFilesLoaded.Text = filesLoaded + " Files Loaded";
        }
        public void blobdetect(Bitmap input)
        {
            Accord.Imaging.BlobCounter blobs = new BlobCounter();
            blobs.FilterBlobs = true;
            blobs.BlobsFilter = null;
            blobs.MinHeight = 35;
            blobs.MinWidth = 35;
            blobs.ProcessImage(input);
            properties[0] = blobs.ObjectsCount;
            textBox1.AppendText("Objectcount: " + blobs.ObjectsCount.ToString() + "\n");
        }
        public void get_background_colors()
        {
            foreach (string fil in emptyImages)
            {
                System.Drawing.Image image = System.Drawing.Image.FromFile(fil);
                Bitmap bitmap = new Bitmap(image);
                Color[] colors = new Color[80000]; 
                arrayMaker.Convert(bitmap,out colors);
                foreach (Color farve in colors)
                {
                    if (!backgrounds.ContainsKey(farve.ToArgb()))
                    {
                        backgrounds.Add(farve.ToArgb(), farve.ToArgb());
                    }
                }
                progressBar1.Increment(1);
            }
        }

        public void get_properties()
        {

        }

        public System.Drawing.Image clean_background(System.Drawing.Image inputImage)
        {
            properties = new int[5];
            int whitePixels = 0;
            progressBar2.Value = 0;
            Bitmap bitmap = new Bitmap(inputImage);
            Color[] colors = new Color[80000];
            Color[] newImage = new Color[80000];
            arrayMaker.Convert(bitmap, out colors);
            int i = 0;
            foreach (Color farve in colors)
            {

                if (backgrounds.ContainsKey(farve.ToArgb()))
                {
                    newImage[i] = Color.Black;
                    
                }
                else
                {
                    newImage[i] = Color.White;
                    whitePixels++;
                }
                i++;
                progressBar2.Increment(1);
            }
            Bitmap outputBitmap;
            imageMaker.Convert(newImage, out outputBitmap);
            blobdetect(outputBitmap);
            properties[1] =whitePixels;
            return outputBitmap;
        }

        public void load_image()
        {
            System.Drawing.Image currentPicture = System.Drawing.Image.FromFile(ballImages[counter]);
            pictureBoxOriginal.Image = currentPicture;
            pictureBoxCleaned.Image = clean_background(currentPicture);
            counter++;
            labelCurrentImage.Text = "Image: " + counter + " / " + filesLoaded;
            labelwhitepixels.Text = "White Pixels: " + properties[1];
        }

        private void buttonLoadIgnores_Click(object sender, EventArgs e)
        {
            get_background_colors();
        }

        private void buttonNextImage_Click(object sender, EventArgs e)
        {
            if ((counter) < filesLoaded)
            {
                load_image();
            }

        }

        private void buttonLoadFiles_Click(object sender, EventArgs e)
        {
            load_files();                       
        }
    }
}
