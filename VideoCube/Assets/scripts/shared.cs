using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

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
    //
    int count;
    int X0;
    int Y0;
    int X1 = -1;
    int Y1 = -1;
    int SQ0 = -1;
    int SQ1 = -1;

    void Start()
    {
        sHandle = new SafeFileHandle(hHandle, true);
        sharedInputCount = 0;
        attachSuccessful = Attach(szMapName, 256);
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
            attachSuccessful = Attach(szMapName, 256);
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
    void FixedUpdate()
    {
        //get Shared memory Input
        if (!attachSuccessful)
        {
            return;
        }
        count = Marshal.ReadInt32(pBuffer, 0);
        Debug.Log($"count={count}");
        X0 = Marshal.ReadInt32(pBuffer, 4);
        Debug.Log($"X0={X0}");
        Y0 = Marshal.ReadInt32(pBuffer, 8);
        Debug.Log($"Y0={Y0}");
        X1 = Marshal.ReadInt32(pBuffer, 12);
        Debug.Log($"X1={X1}");
        Y1 = Marshal.ReadInt32(pBuffer, 16);
        Debug.Log($"Y1={Y1}");
        SQ0 = Marshal.ReadInt32(pBuffer, 20);
        Debug.Log($"SQ0={SQ0}");
        SQ1 = Marshal.ReadInt32(pBuffer, 24);
        Debug.Log($"SQ1={SQ1}");


    }
}