package edu.handong.ghost.ctrlz.nativeclass.recover;

import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: NativeRecover.java
 * @Date	: 1/29, 2013 
 * @작성자	: 이승재
 * @변경이력	:
 * @수정자	: 
 * @설명		: 1. RecoverFactory / (Simple Factory pattern)
 * 		      2. Device에 해당하는 Recover object 생성
 */
public class RecoverFactory {
	
	public static NativeRecover createRecover(Device dev){
		if(dev.getFsType().equals("vfat")){
			return new NativeFatRecover();
		}
		else{
			// throws exception
			return null;
		}
	}
	
}
