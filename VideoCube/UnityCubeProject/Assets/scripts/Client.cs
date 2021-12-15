using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System;
using System.IO;
 
public class Client : MonoBehaviour {
    public int port = 8000;
    string host = "192.168.43.249";
    int n_frame = 0;

    public vccp  protocol;
    public string clientName;
    private bool socketReady;
    public static string response;
    private static byte[] clientBuffer = new byte[1024];

    //creating the socket TCP
    public Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

    //declare end point
    public IPEndPoint conn;
 
 
    // Use this for initialization
    void Start () {

        protocol = GetComponent<vccp>();
        DontDestroyOnLoad(gameObject); //don't destroy the client when moving on to the next scene

    }

    // Algorithm is foolowing:
    // 1. Create Texture 
    // 2. Encode pixels to jpeg data
    // 3. Create texture packet
    //  while( packetCounter < 100 ):            
    //      4. Transfer texture packet
    //      5. Decrease packet counter
    //      6. Change pixels
    //      7. Send another packet
    static byte[] PreapreTestDataForTransfer()
    {
        vccp.CookedData data = vccp.CookTexturePacket();
        byte[] byteData = Encoding.ASCII.GetBytes(data.json_object);
        Debug.Log("message sent to the server " + data.json_object);
        // add pixels data to byteData
        return byteData;
    }
   
    // Update is called once per frame
    void Update () {
        n_frame++;
        if (n_frame == 100)
        {
            port = 8000;
            ConnectToServer(host, port);
        }

        if(n_frame > 100 && (n_frame % 100)  == 0)
            // Send test data to the remote device.
            SendData(clientSocket, "This is a test");


        if (socketReady)
        {
            //check for new messages
            CheckForData(clientSocket);
 
            //Debug.Log("socket ready");
 
        }
       
    }
 
    public bool ConnectToServer(string hostAdd, int port)
    {
        //if already connected ignore this fucntion
        if (socketReady)
        {
            return false;
        }
 
        //connect the socket to the server
        try
        {
            //create end point to connect
            //bind socket
            byte b = 127;
            var ip = 0;
            ip = (1 << 24) + b; // 127.0.0.1
            //string hex = ip.ToString("X2");
            //Debug.Log($"Shifted byte: {Convert.ToString(ip, toBase: 2)}");
            //Debug.Log($"Shifted byte hex: {Convert.ToString(ip, toBase: 16)}");
            //clientSocket.Bind(new IPEndPoint(IPAddress.Parse(host), port+1));

            conn = new IPEndPoint(IPAddress.Parse(host), port);
            //connect to server
            clientSocket.BeginConnect(conn, ConnectCallback, clientSocket);
            socketReady = true;
            Debug.Log("Client socket ready: "+ socketReady);
 
            // Send test data to the remote device.
            //SendData(clientSocket, "This is a test");
            
 
 
            // Receive the response from the remote device.
            //ReceiveData(clientSocket);
 
            CheckForData(clientSocket);
 
            // Write the response to the console
 
        }
        catch (Exception ex)
        {
            Debug.Log("socket error: " + ex.Message);
        }
 
        return socketReady;
    }
 
 
    //async call to connect
    static void ConnectCallback(IAsyncResult ar)
    {
        try
        {
 
            // Retrieve the socket
            Socket client = (Socket)ar.AsyncState;
 
            // Complete the connection
            client.EndConnect(ar);
 
            //Debug.Log("Client successfully connected!!!!!");
            Debug.Log("Client Socket connected to: " + client.RemoteEndPoint);
 
        }
        catch (Exception e)
        {
            Debug.Log("Error connecting: " + e);
        }
    }
 
 
    /////////SEND DATA TO THE SERVER/////////
    //send data to server
    public static void SendData(Socket client, string data)
    {
        byte[] byteData = PreapreTestDataForTransfer();
        Debug.Log($"SendData to Server json:{byteData.Length} bytes");

        //convert the string data to bytes
        //byte[] byteData = Encoding.ASCII.GetBytes(data);
        // Begin sending the data to the remote device.
        client.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallBack), client);
    }
 
    static void SendCallBack(IAsyncResult ar)
    {
        try
        {
            Socket client = (Socket)ar.AsyncState;
 
            //send date to the server
            int bytesSent = client.EndSend(ar);
 
            Debug.Log("client sent: " + bytesSent);
        }
        catch (Exception e)
        {
            Debug.Log("error sending message: " + e);
        }
    }
    //enclose this in one function
 
 
    /////////RECEIVE DATA FROM THE SERVER/////////
 
    //process the data received
    void OnIncomingData(string data)
    {
        Debug.Log("server answer: " + data);
 
 
 
    }
 
    public static void CheckForData(Socket client){
       
        try
        {
            // Begin receiving the data from the remote device.
            client.BeginReceive(clientBuffer, 0, clientBuffer.Length, 0,
                                new AsyncCallback(ReceiveCallback), client);
        }
        catch (Exception e)
        {
            Debug.Log("error receiving the data: " + e.Message);
        }
 
    }
 
    static void ReceiveCallback(IAsyncResult ar)
    {
        try
        {
            // Read data from the remote device.
            Socket client = (Socket)ar.AsyncState;
            int bytesRead = client.EndReceive(ar);
 
            //don't know why after receiving my info this gets called.
            if (bytesRead == 0)
            {
                Debug.Log("no more data to receive");
                return;
            }
 
            var data = new byte[bytesRead];
            Array.Copy(clientBuffer, data, bytesRead);
 
            // Get the data
            client.BeginReceive(clientBuffer, 0, clientBuffer.Length, 0,
                                new AsyncCallback(ReceiveCallback), client);
 
            response = Encoding.Default.GetString(clientBuffer);
 
            Debug.Log("data from server received in the client: " + response);
 
        }
        catch (Exception ex)
        {
            Debug.Log("Error: " + ex.Message);
        }
    }
 
 
 
    /////////CLOSES THE SOCKET/////////
    void OnApplicationQuit()
    {
        CloseSocket();
    }
 
    void OnDisable()
    {
        CloseSocket();
    }
 
 
    void CloseSocket()
    {
 
        if (!socketReady)
        {
            return;
        }
 
        clientSocket.Close();
        socketReady = false;
    }
 
 
 
    public class GameClient
    {
        public string name;
        public bool isHost;
    }
       
}