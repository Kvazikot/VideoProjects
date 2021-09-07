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
using UnityEngine;

public class vccp : MonoBehaviour {
    List<Texture2D> receivedImages;
    List<Texture2D> createdTexures;

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
        if (packet.job == Job.CREATE_NEW_TEXTURE)
        {
            Texture2D texture = new Texture2D(packet.width, packet.height, packet.format, false);
            texture.SetPixels32(pixels);
            GetComponent<Renderer>().material.mainTexture = texture;
            createdTexures.Add(texture);
        }
    }

    void SetCheckBoardPattern(ref Color32[] colors, int width, int height)
    {
        Color32 color = new Color32();
        
        for (int j = height; j > 0; j--)
            for (int i = width; i > 0; i--)
            {
                if ((j % 20) == 0 && (i % 20) == 0)
                    color = new Color(0, 0, 0, 0);
                else
                    color = new Color(64, 41, 45, 255);
                colors[j * width + i] = color;
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

    // Update is called once per frame
    void Update () {
		
	}
}
