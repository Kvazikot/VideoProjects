using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;
using System.IO;

public class shared : MonoBehaviour
{
    //Shared Memory
    [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
    static extern SafeFileHandle OpenFileMapping(
     uint dwDesiredAccess,
     bool bInheritHandle,
     string lpName);

    [DllImport("kernel32.dll", SetLastError = true)]
    static extern IntPtr MapViewOfFile(
    SafeFileHandle hFileMappingObject,
    UInt32 dwDesiredAccess,
    UInt32 dwFileOffsetHigh,
    UInt32 dwFileOffsetLow,
    UIntPtr dwNumberOfBytesToMap);


    /*
        [DllImport("kernel32.dll", SetLastError = true)]
        [ReliabilityContract(Consistency.WillNotCorruptState, Cer.Success)]
        [SuppressUnmanagedCodeSecurity]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool CloseHandle(IntPtr hObject);
    */
    string szMapName = "UnityFileMappingObject";
    const int HEADER_SIZE = 10;
    const int BUFFER_SIZE = 1980 * 1080 * 4 + HEADER_SIZE * 4; //HD FRAME ARGB32 + HEADER SIZE
    byte[] pixels = new byte[BUFFER_SIZE];
    int n_frame = 0;

    const UInt32 STANDARD_RIGHTS_REQUIRED = 0x000F0000;
    const UInt32 SECTION_QUERY = 0x0001;
    const UInt32 SECTION_MAP_WRITE = 0x0002;
    const UInt32 SECTION_MAP_READ = 0x0004;
    const UInt32 SECTION_MAP_EXECUTE = 0x0008;
    const UInt32 SECTION_EXTEND_SIZE = 0x0010;
    const UInt32 SECTION_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED | SECTION_QUERY |
        SECTION_MAP_WRITE |
        SECTION_MAP_READ |
        SECTION_MAP_EXECUTE |
        SECTION_EXTEND_SIZE);
    const UInt32 FILE_MAP_ALL_ACCESS = SECTION_ALL_ACCESS;
    private SafeFileHandle sHandle;
    private IntPtr hHandle;
    private IntPtr pBuffer;
    private int sharedInputCount;
    bool attachSuccessful;
    
    //bytes to read from shared memory
    int count;
    int size;
    int w;
    int h;
    int format;

    void Start()
    {
        sHandle = new SafeFileHandle(hHandle, true);
        sharedInputCount = 0;
        attachSuccessful = Attach(szMapName, BUFFER_SIZE);
    }

    unsafe public bool Attach(string SharedMemoryName, UInt32 NumBytes)
    {
        if (!sHandle.IsInvalid) return false;
        sHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, SharedMemoryName);
        Debug.Log("Shared mem open: ");
        if (sHandle.IsInvalid) return false;
        Debug.Log("Shared mem open SUCCESS: ");
        pBuffer = MapViewOfFile(sHandle, FILE_MAP_ALL_ACCESS, 0, 0, new UIntPtr(NumBytes));
        Debug.Log("Shared mem mapped: ");
        return true;
    }

    unsafe public void Detach()
    {
        if (!sHandle.IsInvalid && !sHandle.IsClosed)
        {
            //CloseHandle(hHandle); //fair to leak if can't close
            sHandle.Close();
        }
        pBuffer = IntPtr.Zero;
        //lBufferSize = 0;
    }
    void Update()
    {
        if (!attachSuccessful)
        {
            attachSuccessful = Attach(szMapName, BUFFER_SIZE);
            return;
        }
    }
    void OnApplicationQuit()
    {
        if (attachSuccessful)
        {
            Detach();
        }
    }

    void CreateTexture(int w, int h, TextureFormat format, byte[] pixels_png)
    {
        Texture2D texture = new Texture2D(w, h, format, false);
         //string fileName = "D:\\projects\\VideoProjects\\VideoCube\\VideoCubeCV\\NicePng_vintage-paper-png_9772625.png";
         //if ( File.Exists(fileName) )
         //   pixels_png = File.ReadAllBytes(fileName);

         // output first 10 bytes of pixels array
        string byte_string = "";
        for (int i = 0; i < 10; i++)
            byte_string += $"{Convert.ToString(pixels_png[i], toBase: 16)},";
        Debug.Log($"pixels_png={byte_string}");


        //if (ImageConversion.LoadImage(texture, pixels_png))
        if ( texture.LoadImage(pixels_png) )
            Debug.Log("PNG decoded!");
        else
            Debug.Log("PNG is NOT decoded!");
        texture.Apply(false);
        GetComponent<Renderer>().material.mainTexture = texture;
        //createdTexures.Add(texture.texture);
        Debug.Log($"Texture created {w}x{h} ");
    }

    void FixedUpdate()
    {
        n_frame++;
        //if (n_frame % 100 == 0)
        {
            //get Shared memory Input
            if (!attachSuccessful)
            {
                return;
            }
            //vccp.CookTexturePacket();
            count = Marshal.ReadInt32(pBuffer, 0);
            //Debug.Log($"count={count}");
            size = Marshal.ReadInt32(pBuffer, 4);
            Debug.Log($"size={size}");
            w = Marshal.ReadInt32(pBuffer, 12);
            Debug.Log($"w={w}");
            h = Marshal.ReadInt32(pBuffer, 16);
            Debug.Log($"h={h}");
            format = Marshal.ReadInt32(pBuffer, 20);
            Debug.Log($"format={format}");

            if (size > 0 && size < BUFFER_SIZE)
            {
                int offset = HEADER_SIZE * 4;
                while (size != 0)
                {
                    pixels[offset - HEADER_SIZE * 4] = Marshal.ReadByte(pBuffer, offset);
                    --size;
                    offset++;
                }
                // output first 10 bytes of pixels
                string byte_string="";
                for (int i = 0; i < 10; i++)
                    byte_string+=$"{Convert.ToString(pixels[i], toBase: 16)},";
                Debug.Log($"pixels={byte_string}");
                //Marshal.Copy(pBuffer + HEADER_SIZE * 4, pixels, 0, size);
                CreateTexture(w, h, TextureFormat.ARGB32, pixels);
            }
        }


    }
}