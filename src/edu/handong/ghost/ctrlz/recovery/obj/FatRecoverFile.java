package edu.handong.ghost.ctrlz.recovery.obj;

/**
 * @FileName: NativeRecover.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: FatRecoverFile class(RecoverFile 하위 클래스)
 */
public class FatRecoverFile extends RecoverFile {
	
	private int firstCluster;
	private byte[] fileName_byte;
	private byte[] dirPath_byte;
	
	public FatRecoverFile() { super(); }
	
	public void setFirstCluster(int firstCluster){
		this.firstCluster = firstCluster;
	}
	public void setFileName_byte(byte[] fname_byte){
		this.fileName_byte = fname_byte;
	}
	public void setDirPath_byte(byte[] dpath_byte){
		this.dirPath_byte = dpath_byte;
	}
	public int getFirstCluster(){
		return firstCluster;
	}
	public byte[] getFileName_byte(){
		return fileName_byte;
	}
	public byte[] getDirPath_byte(){
		return dirPath_byte;
	}
}
