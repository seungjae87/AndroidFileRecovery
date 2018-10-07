package edu.handong.ghost.ctrlz.recovery;

import java.util.ArrayList;

import android.util.Log;

import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

public class FilteringManager {

	public final static int IMAGE 		= 0;
	public final static int AUDIO 		= 1;
	public final static int VIDEO 		= 2;
	public final static int DOC 		= 3;
	public final static int COMPRESS 	= 4;
	public final static int OTHERS 		= 5;
	
	private boolean[] categoryFlags = new boolean[] {false, false, false, false, false, false};
	
	public final static String[][] filterList = {
		{ /* image */	"jpg", "jpeg", "bmp", "png", "gif", "jpe" },
		{ /* audio */	"mp3", "wav" },
		{ /* video */  	"avi", "mpeg", "mp4", "asf", "smi", "mpe"},
		{ /* doc   */  	"doc", "docx", "ppt", "pptx", "hwp", "pdf", "txt" },
		{ /*compress*/  "zip", "ogg", "gz", "tar", "bz", "alg", "rar" }
	};
	
	public boolean[] getCategoryFlags(){
		return this.categoryFlags;
	}
	
	public void setCategoryFlags(boolean[] categoryFlags){
		this.categoryFlags = categoryFlags;
	}
	
	public ArrayList<RecoverFile> filterList(ArrayList<RecoverFile> rlist) throws Exception {
		
		ArrayList<RecoverFile> filteredList = null;
		filteredList = new ArrayList<RecoverFile>();
		RecoverFile tmp;
		
		if(rlist == null){ 
			throw new NullPointerException();
		}
		
		for(int i=0; i<OTHERS; i++){
			if( categoryFlags[i] ){
				for(int j=0; j<rlist.size(); j++){
					tmp = rlist.get(j);
					if(isThisCategory(tmp.getExtender().trim(), i))
						filteredList.add(tmp);
				}
			}
		}
		
		if( categoryFlags[OTHERS] ){
			for(int i=0; i<rlist.size(); i++){
				tmp = rlist.get(i);
				if(isOtherCategory(tmp.getExtender().trim())){
					filteredList.add(tmp);
				}
			}
		}
		
		return filteredList;
	}
	
	private boolean isOtherCategory(String extender){
		int i,j;
		for(i=0; i < 5; i++){
			for(j=0; j<filterList[i].length; j++){
				if(filterList[i][j].equalsIgnoreCase(extender))
					return false;
			}
		}
		return true;
	}
	
	private boolean isThisCategory(String extender, int category){
		for(int i=0; i<filterList[category].length; i++){
			if(filterList[category][i].equalsIgnoreCase(extender))
				return true;
		}
		return false;
	}
	
}
