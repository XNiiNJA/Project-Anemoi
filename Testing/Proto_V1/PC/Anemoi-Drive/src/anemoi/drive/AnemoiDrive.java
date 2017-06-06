/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package anemoi.drive;

import java.awt.MouseInfo;
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
import static java.net.InetAddress.getByAddress;
import java.util.HashSet;
import java.util.Set;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;

import java.awt.PointerInfo;
import java.awt.Robot;


/**
 *
 * @author Grant
 */
public class AnemoiDrive {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws SocketException, UnknownHostException, IOException, InterruptedException, LWJGLException {
        // TODO code application logic here
    
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
           
           
           System.out.println(name);
           
           if(name.contains("Device"))
           {
                  cont = curControl;
                  break;
               
           }
           
        }
        
        byte[] ipAddr = new byte[]{(byte)192, (byte)168, (byte)1, (byte)4};
        InetAddress address = InetAddress.getByAddress(ipAddr);
        
        //Controllers have a -1.0 to 1.0 range for their input. 
        //In order to convert these values to something meaningful to us,
        //we will multiply by 128, then convert to byte. 
        //This way, we can send over the data in byte form. 
        
        //Packet will be structured as...
        //speed|turn_direction
        
        String[] input_array = {"Z Rotation", "X Rotation"};
        
        byte[] output_array = new byte[2];
        
        String outString;
        
        ChannelManager.init("192.168.1.19");
        
        while(true)
        {
            cont.poll();
            
            outString = "-----------------------------------\n";
            
            int k = 0;
            int j = 0;
            //for(int i = 0; i < input_array.length; i++)
            //{
                
                for(; k < cont.getAxisCount(); k++)
                {                    
                    byte axisByteForm = (byte)(cont.getAxisValue(k) * 124);
                    ChannelManager.setChannelValue(k, axisByteForm);
                    outString += cont.getAxisName(k) + ": " + axisByteForm + "\n";//Float.toString(cont.getAxisValue(k)) + "\n"; 
                }

               
                
                for(; j < cont.getButtonCount(); j++)
                {
                    byte buttonByteForm = cont.isButtonPressed(j) ? (byte)1 : (byte)0;
                    ChannelManager.setChannelValue(j + k, buttonByteForm);
                    outString += cont.getButtonName(j) + ": " + buttonByteForm + "\n";//Boolean.toString(cont.isButtonPressed(j)) + "\n";   
                }
                
                                                                                
            //}
            
            try
            {

               if(MouseInfo.getPointerInfo() != null)
               {
                  outString += "Mouse X: " + MouseInfo.getPointerInfo().getLocation().x + "\n";

                  outString += "Mouse Y: " + MouseInfo.getPointerInfo().getLocation().y + "\n";
               }
               
               Robot rob = new Robot();
               
               System.out.println(MouseInfo.getPointerInfo().getDevice());
               
               System.out.println(MouseInfo.getPointerInfo().getDevice().getDisplayMode().getHeight());
               
               System.out.println(MouseInfo.getPointerInfo().getDevice().getDisplayMode().getWidth());
               
            }
            catch(Exception e)
            {
               e.printStackTrace();
               
            }
            
            
            ChannelManager.send();
            
            Thread.yield();
            
            Thread.sleep(3);
            
            System.out.println(outString);
            
            outString = "";
            
            
        }
    
    }
    
}
