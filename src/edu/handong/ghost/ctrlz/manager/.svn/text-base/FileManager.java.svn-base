package edu.handong.ghost.ctrlz.manager;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import android.util.Log;

/**
 * @FileName: FileManager.java
 * @Date : Jan 7, 2013
 * @작성자 : 임의상
 * @변경이력 :
 * @수정자 :
 * @설명 : file을 관리하는 매니저 - singleton 패턴 
 */
public class FileManager {

	private static FileManager fileManager = new FileManager();
	private ArrayList<File> searchList;
	
	private FileManager() {
		return;
	}

	public static FileManager getInstance() {
		return fileManager;
	}

	/**
	 * @Method Name  : sortFilesByFolderAndName
	 * @작성일   : Jan 7, 2013 
	 * @작성자   : 임의상 
	 * @변경이력	:
	 * @Method 설명 : 파일을 폴더순/이름순으로 정렬하여 리턴한다 
	 * @param ArrayList<File>
	 * @return ArrayList<File>
	 */
	public ArrayList<File> sortFilesByFolderAndName(ArrayList<File> files) {

		ArrayList<File> folderList = new ArrayList<File>();
		ArrayList<File> fileList = new ArrayList<File>();
		ArrayList<File> finalList = new ArrayList<File>();
		
		// 폴더와 파일을 나눈다.
		for (int i = 0; i < files.size(); i++) {
			if (files.get(i).isDirectory()) {
				folderList.add(files.get(i));
			} else {
				fileList.add(files.get(i));
			}
		}

		// 정렬
		Collections.sort(folderList, new NameAscCompare());
		Collections.sort(fileList, new NameAscCompare());

		finalList.addAll(folderList);
		finalList.addAll(fileList);
		
		// 파이널 리스트 리턴
		return finalList;
	}

	private static class NameAscCompare implements Comparator<File> {

		// 오름차순 정렬
		@Override
		public int compare(File arg0, File arg1) {
			// TODO Auto-generated method stub
			return arg0.getName().compareTo(arg1.getName());
		}
	}
	
	public void mkdir(String dirPath){
		File file = new File(dirPath);
		int i=1;
		
		while(!file.mkdir()){
			file = new File(dirPath+"("+i+")");
			i++;
		}
	}
	
	public void mvFile(String src, String dest){
		File srcfile = new File(src);
		
		try{
			doEntry_mv(src,dest);
			deleteFile(srcfile);
		}catch(IOException e){
			Log.i("ioerr",e.toString());
		}
	}
	
	private void doEntry_mv(String src, String dest) throws IOException{
		File srcFile = new File(src);
		if(srcFile.isFile()){
			String path = dest.concat("/"+srcFile.getName());
			File newFile = new File(path);
			newFile.createNewFile();
			
			InputStream is = new BufferedInputStream(new FileInputStream(srcFile));
			OutputStream os = new BufferedOutputStream(new FileOutputStream(newFile));
			
			byte[] buffer = new byte[1024];
			
			while(is.read(buffer)!=-1){
				os.write(buffer);
			}
			
			is.close();
			os.close();
			
		}else if(srcFile.isDirectory()){
			doDir_mv(src,dest);
		}
	}
	
	private void doDir_mv(String src, String dest) throws IOException{
		File srcFile = new File(src);
		String dirPath = dest.concat("/"+srcFile.getName());
		mkdir(dirPath);
		
		File newDir = new File(src);
		File[] files = newDir.listFiles();
		
		for(int i=0; i<files.length; i++){
			doEntry_mv(files[i].getPath(),dirPath);
		}
	}
	
	public void cpFile(String src, String dest){
		
		try{
			doEntry_mv(src,dest);
		}catch(IOException e){
			Log.i("ioerr",e.toString());
		}
	}
	
	public String deleteFile(File file){
		String parent = file.getParent();
		doEntry_del(file);
		return parent;
	}
	
	
	private void doEntry_del(File file){
		if(file.isFile()){
			file.delete();
		}else if(file.isDirectory()){
			if(!file.delete()){
				doDir_del(file);
				file.delete();
			}
		}
	}
	
	private void doDir_del(File file){
		File[] fileList = file.listFiles();
		File entry;
		
		for(int i=0; i<fileList.length; i++){
			entry = fileList[i];
			doEntry_del(entry);
		}
	}
	
	public ArrayList<File> searchEntry(String root, String name){
		searchList = new ArrayList<File>();
		File file = new File(root);
		
		doEntry_search(file,name);
		return searchList;
	}
	
	private void doEntry_search(File file, String name){
		if(file.getName().toLowerCase().contains(name.toLowerCase())){
			searchList.add(file);
		}
		
		if(file.isDirectory()){
			doDir_search(file,name);
		}
	}
	
	private void doDir_search(File file,String name){
		File[] fileList = file.listFiles();
		
		for(int i=0; i<fileList.length; i++){
			doEntry_search(fileList[i],name);
		}
	}
}
