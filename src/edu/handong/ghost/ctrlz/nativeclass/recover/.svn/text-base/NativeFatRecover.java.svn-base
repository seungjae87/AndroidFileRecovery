package edu.handong.ghost.ctrlz.nativeclass.recover;

import java.io.IOException;
import java.util.ArrayList;

import android.os.AsyncTask;

import edu.handong.ghost.ctrlz.recovery.obj.Device;
import edu.handong.ghost.ctrlz.recovery.obj.FatRecoverFile;
import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

/**
 * @FileName: RecoveryManager.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: 1. NativeRecover implements(Strategy Pattern)
 * 			  2. FAT FileSystem C Library 연동
 */
public class NativeFatRecover implements NativeRecover {
	
	static { System.loadLibrary("recover"); }
	
	private final String tmpRecoverPath = "/data/data/edu.handong.ghost.ctrlz/TEMP_FILE/";
	
	private native int native_recover_file(String devicePath, String fileName, String mountPoint, int fCluster, long fileSize) throws IOException;
	private native int native_recover_file_tmp(String devicePath, String fileName, String mountPoint, int fCluster, long fileSize) throws IOException;
	private native int native_recover_file_mv( String fileName, String mountPoint) throws IOException;
	private native int native_get_pathcount(String devicePath, String topPath, String mountDir) throws IOException;
	private native ArrayList<RecoverFile> native_scan_path(AsyncTask obj, String devicePath, String topPath, String mountDir) throws IOException;
	private native ArrayList<RecoverFile> native_scan_detail(AsyncTask obj, String devicePath) throws IOException;
	private native int native_delete_file_with_cluster(String devicePath, String mountPoint, byte[] dirPath, byte[] fileName, int fCluster, long size) throws IOException;
	private native int native_delete_file_with_filename(String devicePath, String mountPoint, String dirPath, String fileName) throws IOException;
	private native int native_init_temp() throws IOException;
	
	@Override
	public int getPathCount(Device dev, String topPath) throws IOException {
		int count = 0;
		count = native_get_pathcount(dev.getDevPath(), topPath, dev.getMountDir());
		return count;
	}
	
	@Override
	public String getRecoveryDir(Device dev){
		return dev.getMountDir() + recoverDir;
	}

	@Override
	public ArrayList<RecoverFile> scanPath(AsyncTask obj, Device dev, String topPath) throws IOException {		
		ArrayList<RecoverFile> rlist = 
				native_scan_path(obj, dev.getDevPath(), topPath, dev.getMountDir());		
		return rlist;
	}
	
	public ArrayList<RecoverFile> scanDetail(AsyncTask obj, Device dev) throws IOException{		
		ArrayList<RecoverFile> rlist = 
				native_scan_detail(obj, dev.getDevPath());
		return rlist;
	}
	
	@Override
	public void recoverFile_tmp(Device dev, RecoverFile file) throws IOException {		
		native_recover_file_tmp(dev.getDevPath(), file.getFileName(), dev.getMountDir(), 
								((FatRecoverFile)file).getFirstCluster(), file.getSize());
	}
	
	@Override
	public void recoverFile_mv(Device dev, RecoverFile file) throws IOException {		
		native_recover_file_mv(file.getFileName(), dev.getMountDir());			
	}
	

	@Override
	public void recoverFile(Device dev, RecoverFile file) throws IOException {		
		native_recover_file(dev.getDevPath(), file.getFileName(), dev.getMountDir(),
								((FatRecoverFile)file).getFirstCluster(), file.getSize());				
	}

	@Override
	public void deleteFile(Device dev, RecoverFile file) throws IOException {		
		native_delete_file_with_cluster(dev.getDevPath(), dev.getMountDir(), 
					((FatRecoverFile)file).getDirPath_byte(),((FatRecoverFile)file).getFileName_byte(),
					((FatRecoverFile)file).getFirstCluster(), file.getSize());
	}

	@Override
	public void deleteFile(Device dev, String dirPath, String fileName) throws IOException {		
		native_delete_file_with_filename(dev.getDevPath(), dev.getMountDir(), dirPath, fileName);
	}
	
	@Override
	public void initTemp() throws IOException {			
		native_init_temp();
	}
	
	@Override
	public String getTmpRecoverPath() {		
		return tmpRecoverPath;
	}
	
}
