package edu.handong.ghost.ctrlz.nativeclass.recover;

import java.io.IOException;
import java.util.ArrayList;

import android.os.AsyncTask;

import edu.handong.ghost.ctrlz.recovery.obj.Device;
import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

/**
 * @FileName: NativeRecover.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: NativeRecover Interface 정의 (Strategy pattern)
 */
public interface NativeRecover {
	
	static final String recoverDir= "/RECOVERED/";
	public int getPathCount(Device dev, String topPath) throws IOException;
	public String getRecoveryDir(Device dev);
	public ArrayList<RecoverFile> scanPath(AsyncTask obj, Device dev, String topPath) throws IOException;
	public ArrayList<RecoverFile> scanDetail(AsyncTask obj, Device dev) throws IOException;
	public void recoverFile(Device dev, RecoverFile file) throws IOException;
	public void recoverFile_tmp(Device dev, RecoverFile file) throws IOException;
	public void recoverFile_mv(Device dev, RecoverFile file) throws IOException;
	public void deleteFile(Device dev, RecoverFile file) throws IOException;
	public void deleteFile(Device dev, String dirPath, String fileName) throws IOException;
	public void initTemp() throws IOException;
	public String getTmpRecoverPath();
}
