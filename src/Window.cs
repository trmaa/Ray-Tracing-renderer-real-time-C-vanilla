using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

public class Window : Form {
    public vec2 viewport;
    public vec2 aspectratio;
    public Pixel[] pixel;

    public Window(String title, vec2 size){
        this.Text = title;
        this.Size = new Size((int)size.x, (int)size.y);

        string currentDirectory = Environment.CurrentDirectory;
        string imagePath = System.IO.Path.Combine(currentDirectory, "img", "ico.png");
        Bitmap bitmap = new Bitmap(imagePath);
        this.Icon = Icon.FromHandle(bitmap.GetHicon());

        this.viewport = new vec2((int)128*3.5f,(int)72*3.5f);
        this.aspectratio = new vec2(this.ClientSize.Width,this.ClientSize.Height) / this.viewport;

        this.Paint += (sender, e) => repaint(e.Graphics);

        this.pixel = new Pixel[(int)viewport.x * (int)viewport.y];
        for(int y = 0; y < viewport.y; y++){
            for(int x = 0; x < viewport.x; x++){
                this.pixel[x + y * (int)viewport.x] = new Pixel(Color.FromArgb(255, (int)(255 * y / viewport.y), (int)(255 * x / viewport.x), 0), new vec2(x, y));
            }
        }

        this.DoubleBuffered = true;
    }

    public void print(Graphics g, Color col, vec2 p, vec2 size){
        g.FillRectangle(new SolidBrush(col), p.x, p.y, size.x, size.y);
    }

    public void println(Graphics g, vec2 pointo, vec2 pointf, float thich, Color color){
        Pen pen = new Pen(color, thich);
        g.DrawLine(pen, (int)(pointo.x), (int)(pointo.y), (int)(pointf.x), (int)(pointf.y));
    }

    public void repaint(Graphics g) {
        vec2 size = new vec2(this.ClientSize.Width, this.ClientSize.Height);
        this.aspectratio = size / this.viewport;

        foreach(var p in this.pixel){
            vec2 invertId = this.viewport - p.id;

            App.camara.shader(g, p.id);
            if(p.color != Color.Black)
                this.print(g, p.color, invertId * this.aspectratio-this.aspectratio, this.aspectratio);
        }

        //this.print(g, Color.FromArgb(255, 0, 155, 255), App.camara.project(new vec3(0, 0, 0)), new vec2(10, 10));
        //Cube.render(g);
        Brujula.render(g);
    }
}