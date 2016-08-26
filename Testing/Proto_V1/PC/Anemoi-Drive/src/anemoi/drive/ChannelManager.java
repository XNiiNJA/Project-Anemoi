/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package anemoi.drive;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 *
 * @author Grant
 */
public class ChannelManager {
    
    final static int NUM_CHANNELS = 17;
    
    static byte channelValues[] = new byte[NUM_CHANNELS];
    
    static DatagramSocket socket;
    
    static InetAddress address;
    
    static void init(String ipAddress) throws SocketException, UnknownHostException
    {
        socket = new DatagramSocket(6000);
        address = InetAddress.getByName(ipAddress);
    }
    
    static void init(int numChannels, String ipAddress) throws SocketException, UnknownHostException
    {
        channelValues = new byte[NUM_CHANNELS];
        socket = new DatagramSocket(6000);
        address = InetAddress.getByName(ipAddress);
    }

    /**
     * Send message to controller.
     */
    static void send() throws IOException
    {       
        socket.send(new DatagramPacket(channelValues,0, channelValues.length, address, 6000));
    }
    
    static void setChannelValue(int id, byte value)
    {
        channelValues[id] = value;   
    }
          
    static byte getChannelValue(int id)
    {
        
        return channelValues[id];
        
    }
    
}
