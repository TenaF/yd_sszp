package org.cocos2dx.cpp;

import java.util.List;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.util.Log;

public class AppList {

	static private Activity context = null;

	public AppList(Activity context) {
		this.context = context;
	}

	// 读取已安装的apk
	static public String read() {

		List<PackageInfo> packages = context.getPackageManager().getInstalledPackages(0);
		String str = "";
		for (int i = 0; i < packages.size(); ++i) {
			PackageInfo pg = packages.get(i);
			//Log.i("apk=", "####" + pg.packageName);
			if (pg.packageName.startsWith("com.qianleigame")) {
				str += "|" + pg.applicationInfo.loadLabel(context.getPackageManager()).toString();
			}
		}

		Log.i("已安装的apk:", str);
		return str;
	}

	// 判断是否存在apk
	static public boolean isExistApp(String appName) {
		List<PackageInfo> packages = context.getPackageManager().getInstalledPackages(0);

		String arr[] = appName.split("\\|");
		Log.i("arr[0]=", arr[0]);
		Log.i("arr[1]=", arr[1]);

		for (int i = 0; i < packages.size(); ++i) {
			PackageInfo pg = packages.get(i);
			String tempName = pg.applicationInfo.loadLabel(context.getPackageManager()).toString();

			if (tempName.matches(arr[1]) && pg.packageName.equals(arr[0]))
				return true;
		}
		return false;
	}

	// 打开apk
	static public boolean openApp(String name) {

		String arr[] = name.split("\\|");

		/*Log.d("package=", arr[0]);
		Log.d("apk=", arr[1]);
		Log.d("服务器信息：=", arr[2]);
		Log.d("房间类型=", arr[3]);
		Log.d("账号=", arr[4]);
		Log.d("密码=", arr[5]);
		Log.d("UserID=", arr[6]);
		Log.d("Score=", arr[7]);*/

		Log.i("打开apk=", "####" + arr[0] + " " + arr[1]);
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		ComponentName componentName = new ComponentName(arr[0], "org.cocos2dx.cpp.AppActivity");
		intent.setComponent(componentName);

		intent.putExtra("server", arr[2]);
		intent.putExtra("roomType", arr[3]);
		intent.putExtra("acc", arr[4]);
		intent.putExtra("pwd", arr[5]);
		intent.putExtra("uid", arr[6]);
		intent.putExtra("score", arr[7]);

		try{
			context.startActivity(intent);
			//context.startActivityForResult(intent, 0);
		}catch(Exception e)
		{
			Log.e("Applist ", e.getMessage());
		}
		return true;
	}

}
