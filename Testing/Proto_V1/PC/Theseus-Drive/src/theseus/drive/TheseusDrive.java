/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theseus.drive;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import static java.net.InetAddress.getByAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Controller;
import org.lwjgl.input.Controllers;

/**
 *
 * @author Grant
 */
public class TheseusDrive {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws SocketException, UnknownHostException, IOException, InterruptedException {
        // TODO code application logic here
    
        DatagramSocket socket = new DatagramSocket(6000);
        
    	try {
		Controllers.create();
	} catch (LWJGLException e) {
		e.printStackTrace();
		return;
	}
	System.out.println("Controllers.getControllerCount() = " + Controllers.getControllerCount());
        
        Controller cont = null;
        
        for(int i = 0; i < Controllers.getControllerCount(); i++)
        {
            
           Controller curControl = Controllers.getController(i);
           
           String name = curControl.getName();
           
           if(name.contains("Xbox One"))
           {
               
               cont = curControl;
               break;
               
           }
           
           
        }
        
        byte[] ipAddr = new byte[]{(byte)192, (byte)168, (byte)1, (byte)18};
        InetAddress address = InetAddress.getByAddress(ipAddr);
        
        //Controllers have a -1.0 to 1.0 range for their input. 
        //In order to convert these values to something meaningful to us,
        //we will multiply by 128, then convert to byte. 
        //This way, we can send over the data in byte form. 
        
        //Packet will be structured as...
        //speed|turn_direction
        
        //The speed will be from input "Z Rotation"
        //The turning will be from input "X Rotation"
        
        String[] input_array = {"Z Rotation", "X Rotation"};
        
        byte[] output_array = new byte[2];
        
        while(true)
        {
            cont.poll();
            
            for(int i = 0; i < input_array.length; i++)
            {
                
                for(int k = 0; k < cont.getAxisCount(); k++)
                {
  
                    if(cont.getAxisName(k).equals(input_array[i]))
                    {
                        
                        //Format for output...
                        output_array[i] = (byte) ((cont.getAxisValue(k) + 1)/2 * 255);
                        
                        System.out.println(cont.getAxisValue(k) + 1);
                        
                    }
                    
                }
                
                
            }
            
            
            System.out.println(output_array[0] + "," + output_array[1]);
                        
            
            
            socket.setSoTimeout(100);
            
            socket.send(new DatagramPacket(output_array,0, output_array.length, address, 6000));
            
            Thread.yield();
            
            Thread.sleep(100);
            
        }
    
    }
    
}
