package edu.handong.ghost.ctrlz.nativeclass;

import java.io.IOException;
import java.util.ArrayList;

import edu.handong.ghost.ctrlz.recovery.obj.Device;

public class NativeDeviceList {

	static { System.loadLibrary("getdevice"); }
	public native ArrayList<Device> getDeviceList() throws IOException;
	public native ArrayList<Device> getDeviceList(String type) throws IOException;
}
