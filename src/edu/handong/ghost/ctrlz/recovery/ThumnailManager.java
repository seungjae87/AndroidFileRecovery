package edu.handong.ghost.ctrlz.recovery;

import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

/**
 * @FileName: NativeRecover.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: 미리보기 기능 지원 파일 확장자 정의
 */
public class ThumnailManager {
	
	public static String[] thumnailExtenders = {
			"jpg","jpe","png","gif","bmp"
	};
	
	public static boolean checkToMakeThumnail(RecoverFile file){
		for(int i=0; i<thumnailExtenders.length; i++){
			if(file.getExtender().equalsIgnoreCase(thumnailExtenders[i]))
				return true;
		}
		return false;
	}
}
