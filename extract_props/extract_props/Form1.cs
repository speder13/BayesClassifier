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
        #region Properties

        int counter = 0;
        int filesLoaded = 0;
        int whitePixels = 0;
        int[] properties;

        string[] emptyImages;
        string[] ballImages;

        Bitmap currentBitmap;
        Accord.Imaging.Converters.ImageToArray arrayMaker = new Accord.Imaging.Converters.ImageToArray();
        Accord.Imaging.Converters.ArrayToImage imageMaker = new Accord.Imaging.Converters.ArrayToImage(400,200);
        Dictionary<int, int> backgrounds = new Dictionary<int, int>();

        #endregion Properties

        public Form1()
        {
            InitializeComponent();
            backgrounds.Add(0,0);
        }
                                
        #region Background Colors

        public System.Drawing.Image clean_background(System.Drawing.Image inputImage)
        {
            int i = 0;
            Bitmap bitmap = new Bitmap(inputImage);
            Color[] colors = new Color[80000];
            Color[] newImage = new Color[80000];

            whitePixels = 0;
            progressBar2.Value = 0;
            arrayMaker.Convert(bitmap, out colors);
            
            foreach (Color farve in colors){
                if (backgrounds.ContainsKey(farve.ToArgb())){
                    newImage[i] = Color.Black;
                }
                else{
                    newImage[i] = Color.White;
                    whitePixels++;
                }
                i++;
                progressBar2.Increment(1);
            }
            imageMaker.Convert(newImage, out currentBitmap);
            return currentBitmap;
        }

        public void load_background_colors()
        {
            foreach (string fil in emptyImages)
            {
                System.Drawing.Image image = System.Drawing.Image.FromFile(fil);
                add_ignore_colors(image);
                progressBar1.Increment(1);
            }
        }

        public void add_ignore_colors(System.Drawing.Image image)
        {
            Bitmap bitmap = new Bitmap(image);
            Color[] colors = new Color[80000];
            arrayMaker.Convert(bitmap, out colors);
            foreach (Color farve in colors)
            {
                if (!backgrounds.ContainsKey(farve.ToArgb()))
                {
                    backgrounds.Add(farve.ToArgb(), farve.ToArgb());
                }
            }
            labelIgnoreColors.Text = "Background colors loaded: " + backgrounds.Count;
        }

        public void load_background_image(System.Drawing.Image inputImage)
        {
            pictureBoxBgndOrig.Image = inputImage;
            pictureBoxBgnAfter.Image = clean_background(inputImage);
            add_ignore_colors(inputImage);
            pictureBoxBackgroundCleaned.Image = clean_background(inputImage);
        }

        #endregion Background Colors

        #region Buttons

        private void buttonLoadIgnores_Click(object sender, EventArgs e)
        {
            load_background_colors();
        }

        private void buttonNextImage_Click(object sender, EventArgs e)
        {
            if (checkBoxbackground.Checked)
            {
                load_background_image(System.Drawing.Image.FromFile(ballImages[counter]));
            }
            else
            {
                if ((counter) < filesLoaded)
                {
                    load_image(System.Drawing.Image.FromFile(ballImages[counter]));
                }
            }

        }

        private void buttonLoadFiles_Click(object sender, EventArgs e)
        {
            load_files();
        }

        #endregion Buttons

        #region Misc Loading Functions

        public void load_image(System.Drawing.Image input)
        {
            whitePixels = 0;
            properties = new int[5];
            pictureBoxOriginal.Image = input;
            pictureBoxCleaned.Image = clean_background(input);
            counter++;
            labelCurrentImage.Text = "Image: " + counter + " / " + filesLoaded;
        }

        public void load_files()
        {
            emptyImages = Directory.GetFiles("images/Empty/", "*.*");
            ballImages = Directory.GetFiles("images/Blue/", "*.*"); ;
            progressBar1.Maximum = emptyImages.Count();
            filesLoaded = ballImages.Count();
            progressBar3.Value = 100;
            labelFilesLoaded.Text = filesLoaded + " Files Loaded";
        }

        #endregion Misc Loading Functions

        #region Network Properties

        public void get_properties()
        {
            properties[0] = blobdetect(currentBitmap);  //no of blobs detected
            properties[1] = whitePixels;                //no of white pixels in image
        }

        public int blobdetect(Bitmap input)
        {
            Accord.Imaging.BlobCounter blobs = new BlobCounter();
            blobs.FilterBlobs = true;
            blobs.BlobsFilter = null;
            blobs.MinHeight = 35;
            blobs.MinWidth = 35;

            blobs.ProcessImage(input);

            return (blobs.ObjectsCount);
        }

        #endregion Network Properties


    }
}
