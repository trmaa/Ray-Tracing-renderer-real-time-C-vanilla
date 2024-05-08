using System;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
//using System.Runtime.InteropServices;
//using OpenTK;
//using OpenTK.Graphics;
//using OpenTK.Graphics.OpenGL;

public class Window : Form {
    public vec2 viewport;
    public vec2 aspectratio;
    public Pixel[] pixel;
    public vec3[] lastp;
    public int frames;

    public vec2 scale = new vec2(0,0);

    private readonly object graphicsLock = new object();

    private Bitmap buffer;
    private Graphics bufferGraphics;
    private SolidBrush brush = new SolidBrush(Color.Black);

    public Window(String title, vec2 size){
        this.Text = title;
        this.Size = new Size((int)size.x, (int)size.y);

        this.FormBorderStyle = FormBorderStyle.None;
        this.WindowState = FormWindowState.Maximized;

        this.scale.x = (float)Screen.PrimaryScreen.Bounds.Width / size.x;
        this.scale.y = (float)Screen.PrimaryScreen.Bounds.Height / size.y;

        string currentDirectory = Environment.CurrentDirectory;
        string imagePath = System.IO.Path.Combine(currentDirectory, "img", "ico.png");
        Bitmap bitmap = new Bitmap(imagePath);
        this.Icon = Icon.FromHandle(bitmap.GetHicon());

        this.viewport = size;
        this.aspectratio = new vec2(this.ClientSize.Width, this.ClientSize.Height) / this.viewport;

        this.Paint += (sender, e) => repaint(e.Graphics);

        this.pixel = new Pixel[(int)viewport.x * (int)viewport.y];
        this.lastp = new vec3[(int)viewport.x * (int)viewport.y];
        for(int y = 0; y < this.viewport.y; y++){
            for(int x = 0; x < this.viewport.x; x++){
                this.pixel[x + y * (int)this.viewport.x] = new Pixel(Color.FromArgb(255, (int)(255 * y / viewport.y), (int)(255 * x / viewport.x), 0), new vec2(x, y));
                this.lastp[x + y * (int)this.viewport.x] = new vec3(this.pixel[x + y * (int)this.viewport.x].color.R / 255f, this.pixel[x + y * (int)this.viewport.x].color.G / 255f, this.pixel[x + y * (int)this.viewport.x].color.B / 255f);
            }
        }

        this.DoubleBuffered = true;

        buffer = new Bitmap((int)size.x, (int)size.y);
        bufferGraphics = Graphics.FromImage(buffer);
    }

    public void print(Graphics g, Color col, vec2 p, vec2 size){
        lock (brush) {
            brush.Color = col;
            g.FillRectangle(brush, (p.x-1)*this.scale.x, (p.y-1)*this.scale.y, size.x*this.scale.x, size.y*this.scale.y);
        }
    }

    public void println(Graphics g, vec2 pointo, vec2 pointf, float thich, Color color){
        Pen pen = new Pen(color, thich);
        g.DrawLine(pen, (int)(pointo.x)*this.scale.x, (int)(pointo.y)*this.scale.y, (int)(pointf.x)*this.scale.x, (int)(pointf.y)*this.scale.y);
    }

    public void printToBuffer(Color col, vec2 p, vec2 size){
        lock (brush) {
            brush.Color = col;
            bufferGraphics.FillRectangle(brush, (p.x-1), (p.y-1), size.x, size.y);
        }
    }

    public void printlnToBuffer(vec2 pointo, vec2 pointf, float thich, Color color){
        Pen pen = new Pen(color, thich);
        bufferGraphics.DrawLine(pen, (int)(pointo.x)*this.scale.x, (int)(pointo.y)*this.scale.y, (int)(pointf.x)*this.scale.x, (int)(pointf.y)*this.scale.y);
    }

    public void repaint(Graphics g) {
        g.Clear(Color.Black);

		this.frames = App.camara.moving?1:this.frames+1;
        
        vec2 size = new vec2(this.ClientSize.Width, this.ClientSize.Height);
        this.aspectratio = size / this.viewport;

        this.printToBuffer(Color.FromArgb(255,(int)((Shader.skycolor.x)*255),(int)((Shader.skycolor.y)*255),(int)((Shader.skycolor.z)*255)),new vec2(0,0),size);

        lock (graphicsLock){
            Parallel.ForEach(this.pixel, (p) => {
                vec2 invertId = this.viewport - p.id;
                int index = (int)(p.id.x+p.id.y*this.viewport.x);

                Shader.update(p.id);
                this.lastp[index] = App.camara.moving?new vec3(0,0,0):this.lastp[index]+new vec3(p.color.R, p.color.G, p.color.B);

                Color thecolor;
                if(this.frames<3 || !App.camara.acumulation){
                    thecolor = p.color;
                } else {
					vec3 col = this.lastp[index]/this.frames;
                    thecolor = Color.FromArgb(255,(int)col.x,(int)col.y,(int)col.z);
                }
                
                if(p.color != Color.Black){
                    this.printToBuffer(thecolor, invertId, new vec2(1,1));
                }
            });
        }

        g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
        g.DrawImage(buffer, 0, 0, size.x, size.y);
	}
}
