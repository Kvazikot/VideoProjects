/*+---------------------------------------------------------------------------------------+  <br>
  + - + - + copytight by Kvazikot<br>
  + - + - + email: vsbaranov83 @gmail.com<br>
  + - + - + github: http://github.com/Kvazikot/VideoCube  <br>
  + - - - + - + - -
  Realisation of Video Content Creation Protocol
  Release: 0.1
  Description on russian<br>
+ --------------------------------------------------------------------------------------+ <br>
*/

using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class vccp : MonoBehaviour {
    List<Texture2D> receivedImages = new List<Texture2D>();
    List<Texture2D> createdTexures = new List<Texture2D>();
    public TextAsset imageAsset;

    public class Packet
    {
        string uuid;
        string json_object;
    }

    public enum Job
    {
        CREATE_NEW_TEXTURE = 0,
        EMBED_IN_TEXTURE = 1
    }

    public class CookedData
    {
        public CookedData(string json_object, byte[] jpeg_data)
        {
            this.json_object = json_object;
            this.jpeg_data = jpeg_data;
        }
        string json_object;
        byte[] jpeg_data;
    }

// uncompressed frame
    public class TextureDescrPacket
    {
        public string uuid;
        public int size;
        public int texture_id = -1;
        public int width = 128, height = 128;
        public TextureFormat format = TextureFormat.ARGB32;
        public Job job = Job.CREATE_NEW_TEXTURE;
    }

    CookedData CookTexturePacket(Texture2D tex)
    {
        // Encode texture into PNG
        byte[] bytes = tex.EncodeToJPG();
        TextureDescrPacket pckt = new TextureDescrPacket();
        pckt.uuid = "01701412-0f44-11ec-a466-38b1dbc8b668";
        pckt.texture_id = tex.GetNativeTexturePtr().ToInt32();
        pckt.job = Job.CREATE_NEW_TEXTURE;      
        return new CookedData(JsonUtility.ToJson(pckt), bytes);
    }

    void HandleTexturePacket(TextureDescrPacket packet, Color32[] pixels)
    {
        byte[] pixels1 = new byte[1];
        if (packet.job == Job.CREATE_NEW_TEXTURE)
        {
            

            string fileName = "d:\\projects\\VideoProjects\\examples\\PyEngine3D-master\\Resource\\Externals\\Textures\\sponza\\background.png";
			if ( File.Exists(fileName) )
				pixels1 = File.ReadAllBytes(fileName);
			else
				return;			
			Texture2D texture = new Texture2D(packet.width, packet.height, packet.format, false);
            ImageConversion.LoadImage(texture, pixels1); 
			Debug.Log($"texture format {texture.format}");
			Color32[] pixels2 = new Color32[texture.width * texture.height];
			SetCheckBoardPattern(ref pixels2, texture.width, texture.height);
			texture.SetPixels32(pixels2);
			texture.Apply();
            GetComponent<Renderer>().material.mainTexture = texture;
            createdTexures.Add(texture);
            Debug.Log($"Texture created {texture.GetNativeTexturePtr().ToInt32()}");
        }
    }

    void SetCheckBoardPattern(ref Color32[] colors, int width, int height)
    {
        Color32 color = new Color32();
		
		int num_cells = 20;
		int cell_size = width / num_cells;
        
        for (int j = 0; j < height; j++)
		{
            for (int i = 0; i < width; i++)
            {
                if ((j % cell_size) == 0 || (i % cell_size) == 0)
                    color = new Color32(0, 0, 0, 0);
                else
                    color = new Color32(0, 255, 0, 255);
                colors[j * width + i] = color;
            }
		}
    }

    void Test1ReceiveFromClient()
    {
        TextureDescrPacket packet = new TextureDescrPacket();
        packet.job = Job.CREATE_NEW_TEXTURE;
        packet.width = 256;
        packet.height = 256;
        Color32[] colors = new Color32[packet.width * packet.height];
        SetCheckBoardPattern(ref colors, packet.width, packet.height);
        HandleTexturePacket(packet, colors);
    }

    void Start()
    {
        Test1ReceiveFromClient();
    }
    
    // Update is called once per frame
    void Update () {
		
	}
}
