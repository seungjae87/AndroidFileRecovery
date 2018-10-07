package edu.handong.ghost.ctrlz.recovery;

import java.io.File;
import java.util.ArrayList;

import android.os.AsyncTask;
import android.util.Log;

import edu.handong.ghost.ctrlz.nativeclass.NativeDeviceList;
import edu.handong.ghost.ctrlz.nativeclass.recover.NativeRecover;
import edu.handong.ghost.ctrlz.nativeclass.recover.RecoverFactory;
import edu.handong.ghost.ctrlz.recovery.obj.Device;
import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

/**
 * @FileName: RecoveryManager.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: 복구, 삭제 SDK
 */
public class RecoveryManager {
	
	private static final String[] supportFileSystems = {"vfat"};
	private volatile static Device device;
	private volatile static NativeRecover recover;
	private volatile static ArrayList<Device> devices;
	private volatile static RecoveryManager recoveryManagerObj = null;
	public volatile static FilteringManager filterMgr = new FilteringManager();

	private RecoveryManager(Device device){
		RecoveryManager.device = device;
		recover = RecoverFactory.createRecover(device);
	}
	
	public static synchronized RecoveryManager getInstance(Device device){
		if(recoveryManagerObj == null){
			recoveryManagerObj = new RecoveryManager(device);
			return recoveryManagerObj;
		}		
		if( RecoveryManager.device != device ){
			setDevice(device);
		}
		return recoveryManagerObj;
	}
	
	public static synchronized ArrayList<Device> InitializeRecover() throws Exception {
		Log.d("AndroidRecovery", "InitializeRecover");
		devices = new ArrayList<Device>();
		NativeRecover recover = null;
		
		/* fetch device list */
		for(int i=0; i < supportFileSystems.length; i++){
			devices.addAll(new NativeDeviceList().getDeviceList(supportFileSystems[i]));
		}
		
		/* Initialize device list */
		for(int i=0; i < devices.size(); i++){
			String recoveryDir;
			Device dev = devices.get(i);
			recover = RecoverFactory.createRecover(dev);
			recoveryDir = recover.getRecoveryDir(dev);
			File f_recoverDir = new File(recoveryDir);
			dev.setPathCount(recover.getPathCount(dev, "/"));
			
			/* if recovery dir doesn't exist, make dir */
			if(!f_recoverDir.exists()){
				f_recoverDir.mkdir();
			}
			dev.setRecoveryDir(recoveryDir);
		}
		return devices;
	}

	private static void setDevice(Device device){
		RecoveryManager.device = device;
		recover = RecoverFactory.createRecover(device);
	}

	public Device getDevice() {
		return device;
	}
	
	public static ArrayList<Device> getDeviceList() {	
		return devices;
	}
	
	public int getPathCount(String topPath) throws Exception{
		return recover.getPathCount(device, topPath);
	}
	
	public boolean[] getCategoryFlags(){
		return filterMgr.getCategoryFlags();
	}
	
	public void setCategoryFlags(boolean[] categoryFlags){
		filterMgr.setCategoryFlags(categoryFlags);
	}

	public ArrayList<RecoverFile> scanPath(AsyncTask obj, String topPath) throws Exception {
		ArrayList<RecoverFile> rlist = recover.scanPath(obj, device, topPath);		
		return filterMgr.filterList(rlist);
	}
	
	public ArrayList<RecoverFile> scanDetail(AsyncTask obj) throws Exception {
		ArrayList<RecoverFile> rlist = recover.scanDetail(obj, device);		
		return filterMgr.filterList(rlist);
	}

	public void genThumnails(ArrayList<RecoverFile> list) throws Exception {		
		initTemp();
		for(int i=0; i<list.size(); i++){
			if(ThumnailManager.checkToMakeThumnail(list.get(i))){
				recover.recoverFile_tmp(device, list.get(i));
				list.get(i).setUrl(recover.getTmpRecoverPath() + list.get(i).getFileName());
				list.get(i).setThumnailGenFlag();
			}
		}
	}

	public void recoverFile(ArrayList<RecoverFile> rlist) throws Exception {
		for(int i=0; i < rlist.size(); i++){
			if(rlist.get(i).isThumnailGen())
				recover.recoverFile_mv(device, rlist.get(i));
			else
				recover.recoverFile(device, rlist.get(i));
		}
		initTemp();
	}

	public void deleteFile(RecoverFile file) throws Exception {		
		recover.deleteFile(device, file);
	}

	public void deleteFile(String filePath) throws Exception {
		
		// check mount point is included or not
		// split filename and directory path
		if(filePath.contains(device.getMountDir())){
			
			String fileName = filePath.substring(filePath.lastIndexOf("/"));
			String dirPath = filePath.substring(filePath.lastIndexOf(device.getMountDir()), 
												filePath.lastIndexOf("/"));			
			recover.deleteFile(device, dirPath, fileName);
		}
		else{
			String fileName = filePath.substring(filePath.lastIndexOf("/"));
			String dirPath = filePath.substring(0, filePath.lastIndexOf("/"));			
			recover.deleteFile(device, dirPath, fileName);
		}
	}

	public void initTemp() throws Exception {
		recover.initTemp();
	}

}
