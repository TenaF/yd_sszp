/****************************************************************************

 ****************************************************************************/
package org.cocos2dx.cpp;

import android.util.Log;
import android.app.Application;
import android.content.Context;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Environment;
import android.os.Vibrator;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import java.util.Random;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import java.io.File;
import java.security.MessageDigest;
import android.os.Bundle;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;

public class QLUtil {

	private final static String TAG = "QLUtil";
	public static Context context = null;

	public static void vibrate() {
		Log.i(TAG, "vibrate");

		Vibrator vibrator = (Vibrator) Cocos2dxActivity.getContext().getSystemService(Context.VIBRATOR_SERVICE);
		long[] pattern = { 0, 300 }; // 不震动时间、震动时间
		vibrator.vibrate(pattern, -1); // -1 表示不重复
	}

	// --------- 信号监控 开始 ---------
	private static int m_nGPRSSignal = 0; // 0~100
	private static int m_nWifiSignal = 0; // 0~100
	// private static GPRSSignalReceive m_gprsReceive = null;
	// private static WIFISignalReceive m_wifiReceive = null;
	// private static NetStateReceive m_netstateReceive = null;
	private static final int NET_TYPE_NONE = 0;
	private static final int NET_TYPE_GPRS = 1;
	private static final int NET_TYPE_WIFI = 2;
	private static int m_nNetType = NET_TYPE_NONE;

	// 0~-100，越大越好，0 最强，-70 就基本连不上了
	private static class WIFISignalReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			int value = intent.getExtras().getInt("newRssi");
			int temp = value + 100;
			if (m_nWifiSignal != temp) {
				m_nWifiSignal = temp;
			}
			Log.i(TAG, "WIFI, onReceive, value=" + value + ", m_nWifiSignal=" + m_nWifiSignal);
		}
	}

	// (0-31, 99) as defined in TS 27.007 8.5
	private static class GPRSSignalReceiver extends PhoneStateListener {

		@Override
		public void onSignalStrengthsChanged(android.telephony.SignalStrength signalStrength) {
			super.onSignalStrengthsChanged(signalStrength);
			int value = signalStrength.getGsmSignalStrength();
			int temp = value * 100 / 31;
			if (m_nGPRSSignal != temp) {
				m_nGPRSSignal = temp;
			}
			Log.i(TAG, "GPRS, onReceive, value=" + value + ", m_nGPRSSignal=" + m_nGPRSSignal);
		}
	}

	private static class NetStateReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			Log.i(TAG, "NetState, onReceiver");

			m_nNetType = NET_TYPE_NONE;
			// 获得网络连接服务
			ConnectivityManager connManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
			// 获取WIFI网络连接状态
			if (connManager != null) {

				NetworkInfo wifiTypeInfo = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
				NetworkInfo mobileTypeInfo = connManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);

				if (wifiTypeInfo != null && mobileTypeInfo != null) {

					// 判断是否正在使用WIFI网络
					if (State.CONNECTED == wifiTypeInfo.getState() && mobileTypeInfo.getState() != State.CONNECTED) {
						m_nNetType = NET_TYPE_WIFI;
					}
					// 判断是否正在使用GPRS网络
					if (State.CONNECTED == mobileTypeInfo.getState() && wifiTypeInfo.getState() != State.CONNECTED) {
						m_nNetType = NET_TYPE_GPRS;
					}
				}
			}
			Log.i(TAG, "NetState, onReceiver, m_nNetType=" + m_nNetType);
		}
	}

	public static void registerSignal(Context context) {
		Log.i(TAG, "registerSignal");

		// Connect
		NetStateReceiver netstateReceiver = new NetStateReceiver();
		IntentFilter filter = new IntentFilter();
		filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		context.registerReceiver(netstateReceiver, filter);

		// WIFI
		WIFISignalReceiver wifiReceiver = new WIFISignalReceiver();
		filter = new IntentFilter(WifiManager.RSSI_CHANGED_ACTION);
		context.registerReceiver(wifiReceiver, filter);

		// GPRS
		TelephonyManager telManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		GPRSSignalReceiver gprsReceiver = new GPRSSignalReceiver();
		telManager.listen(gprsReceiver, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);
	}

	public static int getSignal() {
		int signal = 0;
		if (m_nNetType == NET_TYPE_WIFI) {
			signal = m_nWifiSignal;
		} else if (m_nNetType == NET_TYPE_GPRS) {
			signal = m_nGPRSSignal;
		}
		Log.i(TAG, "getSignal, type=" + m_nNetType + ", signal=" + signal);
		return signal;
	}

	// --------- 信号监控 结束 ---------

	// --------- 电池监控 开始 ---------
	private static BatteryReceiver m_batteryReceiver = null;
	private static int m_nBatteryPercent = 90;

	private static class BatteryReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {

			if (intent != null) {
				Bundle bundle = intent.getExtras();
				if (bundle != null) {
					int current = bundle.getInt("level");// 获得当前电量
					int total = bundle.getInt("scale");// 获得总电量
					Log.i(TAG, "Battery, onReceive, current=" + current + ", total=" + total);
					if (total != 0) {
						m_nBatteryPercent = current * 100 / total;
					}
				}
			}
		}
	}

	public static void registerBattery(Context context) {
		Log.i(TAG, "registerBattery");
		m_batteryReceiver = new BatteryReceiver();
		IntentFilter filter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		context.registerReceiver(m_batteryReceiver, filter);
	}

	public static int getBattery() {
		Log.i(TAG, "getBattery m_nBatteryPercent=" + m_nBatteryPercent);
		return m_nBatteryPercent;
	}

	// --------- 电池监控 结束 ---------

	/**
	 * 获取设备mac地址
	 * 
	 * @return mac地址
	 */

	public static String getMacAddress() {
		WifiManager wifi = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
		String macAddr = info.getMacAddress();
		if (macAddr == null || macAddr.length() != 17) {
			Random r = new Random();
			long l = r.nextLong();
			String str = Long.toHexString(l);
			StringBuffer sb = new StringBuffer();

			for (int i = 0; i < 6; i++) {
				if (i > 0) {
					sb.append(":");
				}
				sb.append(str.subSequence(i * 2, i * 2 + 2));
			}
			macAddr = sb.toString();
		}
		return macAddr == null ? "" : macAddr.toUpperCase();
	}

	/**
	 * 获取国际移动设备身份码
	 * 
	 * @return IMEI
	 */
	public static String getIMEI() {
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		String IMEI = null;
		if (tm != null) {
			IMEI = tm.getDeviceId();
			if (IMEI == null || IMEI.length() == 0) {
				IMEI = tm.getSubscriberId();

				if (IMEI == null || IMEI.length() == 0) {

					IMEI = tm.getSimSerialNumber();
					if (IMEI == null || IMEI.length() == 0) {
						Random r = new Random();
						long l = r.nextLong();
						String str = Long.toHexString(l);
						IMEI = str.substring(0, 15);
					}
				}
			}
		}
		return IMEI == null ? "" : IMEI.toUpperCase();
	}

	private static final char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	private static String byteToHexString(byte[] data) {
		char chars[] = new char[16 * 2];
		int k = 0;
		for (byte b : data) {
			chars[k++] = hexdigits[b >>> 4 & 0x0F];
			chars[k++] = hexdigits[b & 0x0F];
		}
		return new String(chars);
	}

	public static String getMD5(String s) {
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(s.getBytes());
			byte[] key = md.digest();
			return byteToHexString(key);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	/*
	 * 得到uuid
	 */
	// private static String uuidStr = "";
	public static String getUniqueId() {
		Log.i(TAG, "getUniqueId");
		String str = "";
		String imei = getIMEI();
		if (imei == "") {
			String mac = getMacAddress();
			str = getMD5(mac);
		} else {
			str = getMD5(imei);
		}
		return str;
	}

	/**
	 * SD卡是否存
	 * 
	 * @return
	 */
	public static boolean checkSDcard() {
		boolean flag = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
		// if (!flag) {
		// Toast.makeText(m_WeiLe, "请插入手机存储卡再使用本功能", Toast.LENGTH_SHORT)
		// .show();
		// }
		return flag;
	}

	// 升级文件
	public static void updateVersion(String name) {

		try {
			String permission = "666";
			String command = "chmod " + permission + " " + name;
			Runtime runtime = Runtime.getRuntime();
			runtime.exec(command);

			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setDataAndType(Uri.fromFile(new File(name)), "application/vnd.android.package-archive");
			//intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(intent);

		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
	
	private static PackageInfo getPackageInfo(Context context) {
	    PackageInfo pi = null;
	    //PackageManager pm = context.getPackageManager();
	    try {
	        PackageManager pm = context.getPackageManager();
	        pi = pm.getPackageInfo(context.getPackageName(),
	                PackageManager.GET_CONFIGURATIONS);
	 
	        return pi;
	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	 
	    return pi;
	}
	 /**
     * 获取版本号
     * @return
     */
    public static int getVersionCode(){
    	return getPackageInfo(context).versionCode;
    }

    /**
     * 获取版本名
     * @return
     */
    public static String getVersionName(){
    	 return getPackageInfo(context).versionName;
    }

	// 手机号
	public static String getPhoneNumber() {
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		String tel = tm.getLine1Number();
		if(tel != null && tel.indexOf("+86") > -1)
			tel = tel.substring(3);
		
		Log.i("getPhoneNumber", tel);
		return tel;
	}

	 // 手机型号
	public static String getPhoneModel() {
		String model= android.os.Build.BRAND + android.os.Build.MODEL ;  
		return model;
	}

	// android系统版本号
	public static String getSystemVersion() {
		String release=android.os.Build.VERSION.RELEASE;  
		return release;
	}

	/**
     * 判断当前手机是否有ROOT权限
     * @return
     */
    public static boolean isRoot(){
        boolean bool = false;

        try{
            if ((!new File("/system/bin/su").exists()) && (!new File("/system/xbin/su").exists())){
                bool = false;
            } else {
                bool = true;
            }
            Log.d(TAG, "bool = " + bool);
        } catch (Exception e) {

        } 
        return bool;
    }

    //uuid
    public static String getUUID() {

    	return Installation.id(context);
    }

}
