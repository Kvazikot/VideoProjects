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
        public string json_object;
        public byte[] jpeg_data;
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

    static public CookedData CookTexturePacket()
    {
        // Encode texture into PNG
        ChekerBoardTexture checkerboard = new ChekerBoardTexture();
        checkerboard.InitTexture();
        checkerboard.Generate();

        Texture2D tex = checkerboard.texture;
        byte[] bytes = tex.EncodeToJPG();
        TextureDescrPacket pckt = new TextureDescrPacket();
        pckt.uuid = "01701412-0f44-11ec-a466-38b1dbc8b668";
        pckt.texture_id = tex.GetNativeTexturePtr().ToInt32();
        pckt.job = Job.CREATE_NEW_TEXTURE;      
        return new CookedData(JsonUtility.ToJson(pckt), bytes);
    }


    void HandleTexturePacketFromClient(TextureDescrPacket packet, Color32[] pixels)
    {
        byte[] pixels1 = new byte[1];
        if (packet.job == Job.CREATE_NEW_TEXTURE)
        {
            // string fileName = "d:\\projects\\VideoProjects\\examples\\PyEngine3D-master\\Resource\\Externals\\Textures\\sponza\\background.png";
            // if ( File.Exists(fileName) )
            // pixels1 = File.ReadAllBytes(fileName);
            // else
            // return;			
            //ImageConversion.LoadImage(texture, pixels1); 
            ProcTexture texture = new ProcTexture();
            texture.setParametres(packet.width, packet.height, packet.format);
            texture.InitTexture();
            GetComponent<Renderer>().material.mainTexture = texture.texture;
            createdTexures.Add(texture.texture);
            Debug.Log($"Texture created {texture.texture.GetNativeTexturePtr().ToInt32()}");
        }
    }

    void Test1ReceiveFromClient()
    {
        TextureDescrPacket packet = new TextureDescrPacket();
        packet.job = Job.CREATE_NEW_TEXTURE;
        packet.width = 256;
        packet.height = 256;
        Color32[] colors = new Color32[packet.width * packet.height];
        HandleTexturePacketFromClient(packet, colors);
    }

    void Start()
    {
        Test1ReceiveFromClient();
    }
    
    // Update is called once per frame
    void Update () {
		
	}
}
