package com.skyetek.demo;

import com.skyetek.comm.*;
import com.skyetek.reader.*;
import java.util.*;
/**
 * SkyeTek Reader Test - Connects to a Reader on COM port 4 and asks it to read tags
 * <P>
 * @version 1.0 April 2002
 * @author Chris Parkinson
 */

public class SkyeTekReaderTest {


/**
 * Constructor
 */
public SkyeTekReaderTest() throws Exception {

    SkyeTekReader reader = null;

	Properties props = new Properties();
        byte [] empty = new byte[0];
	props.setProperty("SkyeTekImpl","com.skyetek.comm.SkyeTekSerialComm");
	System.setProperty("com.skyetek.debug","true");

        try {
	    reader = new SkyeTekReader(props);
	    Thread.sleep(20);

	    System.out.println("Started reader...");
	    System.out.println("querySerialNumber()");
	    //reader.querySerialNumber();
	    //System.out.println("queryFirmware() ");
	    //reader.queryFirmware();
	    //System.out.println("queryReaderID(RAM)");
	    //reader.queryReaderID(RAM);

       //     reader.updateReaderID(RAM, (byte)0xFF);


	}
	catch(Exception ex)
	{
		ex.printStackTrace();
		System.out.println("Exception occured ....");
	}
	System.out.println("Start Reading Tags .....");

    //Ask the reader to read tags
    boolean foundTags = false;
    while(!foundTags)
    {
		Thread.sleep(1000);
		Collection tagList = reader.getTagList();
		if (tagList==null || tagList.size() == 0)System.out.println("No Tags Found");
		else{
		for (Iterator itr=tagList.iterator(); itr.hasNext();)
		{
			SkyeTekTag tag = (SkyeTekTag)itr.next();
			System.out.println("Tag Found: " + tag.getTagID());
			foundTags = true;
		}
	}
    }

    //Close the connection
    reader.close();
}

/**
 * Main
 */
 public static final void main(String args[]){
    try{
  
	SkyeTekReaderTest test = new SkyeTekReaderTest();
   }catch(Exception e){
	System.out.println("Error: "+e.toString());
    }
}

}