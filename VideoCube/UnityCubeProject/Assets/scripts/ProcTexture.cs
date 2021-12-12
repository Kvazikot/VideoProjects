using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProcTexture  {

    public Texture2D texture;
    public Color32[] pixels;
    public Dictionary<string, string> parametres = new Dictionary<string, string>();
    public Dictionary<string, string> parametres_old = new Dictionary<string, string>();

    public ProcTexture()
    {
        parametres = new Dictionary<string, string>();
        parametres["width"] = Convert.ToString(128, toBase: 10);
        parametres["height"] = parametres["width"];
        TextureFormat format = TextureFormat.ARGB32;
        parametres["format"] = format.ToString();

        //{"num_cells", "20"}, {"color1", "255,0,0,5"}};
    }

    public virtual void Generate()    {    }

    bool isneedReinit()
    {
        int w = int.Parse(parametres["width"]);
        int h = int.Parse(parametres["height"]);
        return ((w != texture.width) || (h != texture.height));
    }

    public void InitTexture()
    {
        int w = int.Parse(parametres["width"]);
        int h = int.Parse(parametres["height"]);
//! TODO PARSE TEXTURE FORMAT
        texture = new Texture2D(w, h, TextureFormat.ARGB32, false);
        Debug.Log($"texture format {texture.format}");
        pixels = new Color32[texture.width * texture.height];
        texture.SetPixels32(pixels);
        texture.Apply(false);
        Debug.Log($"Texture created {texture.GetNativeTexturePtr().ToInt32()}");
    }

    public void Reinit()
    {
        if (isneedReinit())
            InitTexture();
    }

    public void setParametres(int width, int height, TextureFormat format)
    {
        parametres["width"] = width.ToString();
        parametres["height"] = height.ToString();
        parametres["format"] = format.ToString();
        Debug.Log($"Texture format {format.ToString()}");
    }

    public static Color32 ParseColor(string colostr)
    {
        string[] parts = colostr.Split(',');
        if (parts.Length != 3) return new Color32();
        byte[] c = new byte[4];
        for (int i = 0; i < 3; i++) c[i] = byte.Parse(parts[i]);
        return new Color32(c[0], c[1], c[2], c[3]);
    }

};

public class ChekerBoardTexture :  ProcTexture
{

    public ChekerBoardTexture() 
    {
        parametres.Add("num_cells", "20");
        parametres.Add("color1", "255,0,0,255");
        parametres.Add("color2", "255,0,255,255");
    }

    public override void Generate() 
    {
        Color32 color1 = ParseColor(parametres["color1"]);
        Color32 color2 = ParseColor(parametres["color2"]);
        int num_cells = int.Parse(parametres["num_cells"]);
        int cell_size = texture.width / num_cells;
        bool White = true;
        for (int j = 0; j < texture.height; j++)
        {
            if ((j % cell_size) == 0)
                White = !White;
            for (int i = 0; i < texture.width; i++)
            {
                if ((i % cell_size) == 0)
                    White = !White;
                if (White)
                    pixels[j * texture.width + i] = color1;
                else
                    pixels[j * texture.width + i] = color2;
            }
        }
        parametres_old = parametres;
    }
}