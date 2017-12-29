/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

//import org.cocos2dx.cpp.weixin.WeiXinUtils;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ImageView.ScaleType;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {

	static private native boolean onNotifyOpenUrlStart(int nPointToWebView, String strUrl);

	static private native void onNotifyOpenUrlComplete(int nPointToWebView);

	static private native boolean onNotifyOpenUrlError(int nPointToWebView, String strError);

	static public native void UpdateDownSchedule(int nPointLoginUIScene, int nCount, int nDownFilePosition);

	static private native void OnWxShareReply(int nCode);

	static private native void onNotifyRefreshScore();

	static private native void onNotifyReconnectServer(); // 重新连接服务器

	static private WebView m_WebView;
	static private ProgressDialog pro;
	static int nDownFilePosition;
	static ValueCallback<Uri> mUploadMessage = null;
	static private AppActivity m_WeiLe;
	RelativeLayout m_webLayout;

	static private AppList list = null;
	static private AppActivity context = null;
	
	public static void requestAction(int actionType,String str)
	{
	    context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    switch(actionType)
	    {
	    case 1://横屏
	    	context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    break;
	    case 2://竖屏
	    	context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    break;
	    }
	}
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		context = this;
		list = new AppList(this);
		QLUtil.registerBattery(this);
		QLUtil.registerSignal(this);
		QLUtil.context = this;
		
		//WeiXinUtils.getInstance(this);
		return glSurfaceView;
	}

	protected void onCreate(final Bundle saveInstanceState) {
		super.onCreate(saveInstanceState);
		try {

			m_WeiLe = this;
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
			m_webLayout = new RelativeLayout(this);
			addContentView(m_webLayout, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 打开apk返回结果
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.i("onActivityResult", "--接受到启动apk的返回值 requestCode:" + requestCode + " resultCode:" + resultCode);
		switch (resultCode) {
		case 1: {
			// onNotifyReconnectServer();
		}
			break;
		default:
			break;
		}
	}

	static public class WebCallBackClient extends WebViewClient {
		private int m_nPointToWebView;

		public WebCallBackClient(int nPointToWebView) {
			super();
			m_nPointToWebView = nPointToWebView;
		}

		public WebCallBackClient() {

		}

		public boolean shouldOverrideUrlLoading(WebView view, final String url) {

			Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {

				@Override
				public void run() {

					boolean b = AppActivity.onNotifyOpenUrlStart(m_nPointToWebView, url);

					System.out.println(url);

					if (b) {
						Message msg = new Message();
						msg.what = 10;
						Bundle data = new Bundle();
						data.putBoolean("OpenUrlStartReturn", b);
						data.putString("url", url);
						msg.setData(data);
						handler.sendMessage(msg);
					}

				}
			});

			return true;
		}

		public void onPageFinished(WebView view, String url) {
			// 关闭等待
			if (pro != null) {
				pro.dismiss();
			}

			if (AppActivity.onNotifyOpenUrlStart(m_nPointToWebView, url)) {
				AppActivity.onNotifyOpenUrlComplete(m_nPointToWebView);
			}
		}

		public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
			if (pro != null) {
				pro.dismiss();
			}

			if (AppActivity.onNotifyOpenUrlStart(m_nPointToWebView, failingUrl)) {
				AppActivity.onNotifyOpenUrlError(m_nPointToWebView, description);
			}

		}

		public void onPageStarted(WebView view, String url, Bitmap favicon) {

			if (!AppActivity.onNotifyOpenUrlStart(m_nPointToWebView, url)) {
				view.stopLoading();
			}

		}

	}

	static public class WebCallBackChromeClient extends WebChromeClient {

		public void openFileChooser(ValueCallback<Uri> uploadFile, String acceptType, String capture) {
			mUploadMessage = uploadFile;
		}
	}

	static private class WebRunnable implements Runnable {
		private int m_nPointToWebView;
		private String m_strUrl;
		private int m_nLeft;
		private int m_nTop;
		private int m_nWidth;
		private int m_nHeight;

		public WebRunnable(int nPointToWebView, String strUrl, int nLeft, int nTop, int nWidth, int nHeight) {
			super();
			m_nHeight = nHeight;
			m_nWidth = nWidth;
			m_nTop = nTop;
			m_nLeft = nLeft;
			m_strUrl = strUrl;
			m_nPointToWebView = nPointToWebView;
		}

		@SuppressLint("SetJavaScriptEnabled")
		public void run() {
			try {
				// 背景遮罩
				RelativeLayout backgroundLayout = new RelativeLayout(m_WeiLe);
				backgroundLayout.setBackgroundColor(Color.GRAY);
				backgroundLayout.getBackground().setAlpha(150);

				backgroundLayout.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View arg0) {

					}
				});

				m_WeiLe.m_webLayout.addView(backgroundLayout, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

				pro = ProgressDialog.show(m_WeiLe, null, null);
				m_WebView = new WebView(m_WeiLe);
				WebSettings webSetting = m_WebView.getSettings();
				webSetting.setJavaScriptEnabled(true);
				webSetting.setSupportZoom(true);
				webSetting.setBuiltInZoomControls(false);
				webSetting.setUseWideViewPort(false);
				webSetting.setLoadWithOverviewMode(true);
				webSetting.setCacheMode(WebSettings.LOAD_NO_CACHE);
				webSetting.setUserAgentString("WeileApp:yes");
				webSetting.setSavePassword(false);
				webSetting.setSaveFormData(false);
				m_WebView.setWebChromeClient(new WebCallBackChromeClient());

				m_WebView.setWebViewClient(new WebCallBackClient(m_nPointToWebView));
				m_WebView.loadUrl(m_strUrl);
				m_WebView.requestFocus();
				m_WebView.setBackgroundColor(0);

				Display mDisplay = m_WeiLe.getWindowManager().getDefaultDisplay();
				int w = mDisplay.getWidth();
				int h = mDisplay.getHeight();

				RelativeLayout.LayoutParams weblatouParmas = null;
				// 根据不同网页类型设置不同大小的webview

				// 添加背景
				ImageView img = new ImageView(m_WeiLe);
				img.setBackgroundDrawable(Drawable.createFromStream(m_WeiLe.getAssets().open("webview/bj.png"), null));

				RelativeLayout.LayoutParams latouParmas = new RelativeLayout.LayoutParams(w, h);
				latouParmas.addRule(RelativeLayout.CENTER_IN_PARENT);
				img.setLayoutParams(latouParmas);
				m_WeiLe.m_webLayout.addView(img);

				// 常用网页
				weblatouParmas = new RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
				weblatouParmas.setMargins(0, 80, 0, 0);

				// 添加关闭按钮
				ImageButton but = new ImageButton(m_WeiLe);
				RelativeLayout.LayoutParams butlatouParmas = new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
				butlatouParmas.addRule(RelativeLayout.ALIGN_PARENT_TOP);
				butlatouParmas.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, RelativeLayout.TRUE);

				Drawable drawable = Drawable.createFromStream(m_WeiLe.getAssets().open("webview/btn_close_n.png"), null);
				but.setImageDrawable(drawable);
				but.setBackgroundColor(0);
				but.setScaleType(ScaleType.CENTER_INSIDE);
				but.setOnClickListener(new OnClickListener() {

					public void onClick(View arg0) {
						// 关闭网页
						m_WeiLe.m_webLayout.removeAllViews();
					}
				});

				weblatouParmas.addRule(RelativeLayout.CENTER_IN_PARENT);
				m_WebView.setLayoutParams(weblatouParmas);
				m_WeiLe.m_webLayout.addView(m_WebView);
				m_WeiLe.m_webLayout.addView(but, butlatouParmas);

			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	}

	static final int MSG_UPDATE_APP = 0;
	static final int MSG_WXLINK = 11;

	static android.os.Handler handler = new android.os.Handler() {
		public void handleMessage(android.os.Message msg) {

			if (msg.what == 10) {
				Bundle bundle = msg.getData();
				m_WebView.loadUrl(bundle.getString("url"));
			}

		};
	};

	public LinearLayout onCreateLayout(Cocos2dxGLSurfaceView surfaceView) {
		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);
		layout.addView(surfaceView);
		return layout;
	}

	static public void openUrl(int nPointWebView, String strUrl, int nLeft, int nTop, int nWidth, int nHeight) {
		m_WeiLe.runOnUiThread(new WebRunnable(nPointWebView, strUrl, nLeft, nTop, nWidth, nHeight));
	}
	static private void handleShowWeb() {
		//m_WebView
		m_WeiLe.m_webLayout.setVisibility(View.VISIBLE);//可见 但是存在
		
	}
	
	static private void handleHideWeb() {
		
		//m_WebView
		m_WeiLe.m_webLayout.setVisibility(View.INVISIBLE);//不可见 但是存在
		
	}
	static private void removeWebView() {
		m_WeiLe.m_webLayout.removeView(m_WebView);
		m_WebView.destroy();
	}

	static public void destroyWeb() {
		m_WeiLe.runOnUiThread(new Runnable() {
			public void run() {
				removeWebView();
			}
		});
	}

	static public void showWeb() {
		m_WeiLe.runOnUiThread(new Runnable() {
			public void run() {
				handleShowWeb();
			}
		});
	}
	
	static public void hideWeb() {
		m_WeiLe.runOnUiThread(new Runnable() {
			public void run() {
				handleHideWeb();
			}
		});
	}
	
	
	
	//改写物理按键——返回的逻辑
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // TODO Auto-generated method stub
    	System.out.println("onKeyDown------------------");
        if(keyCode==KeyEvent.KEYCODE_BACK)
        {
            if(m_WebView.canGoBack())
            {
            	m_WebView.goBack();//返回上一页面
                return true;
            }
            else
            {	
            	removeWebView();
                //System.exit(0);//退出程序
            	return true;
            }
        }
        return super.onKeyDown(keyCode, event);
    }

	public static void ipay(String score, String acc) {
		/*
		Intent intent = new Intent();
		intent.setClass(m_WeiLe, IPay.class);
		Bundle mBundle = new Bundle();
		mBundle.putString("account", acc);// 压入数据
		mBundle.putString("score", score);// 用户金币
		intent.putExtras(mBundle);
		*/
		//m_WeiLe.startActivity(intent);

	}

	public static void notifyReconnect() {
		onNotifyReconnectServer();
	}

	static public void weixinShare(String path) {
		Log.i("weixinShare", path);

		//WeiXinUtils.getInstance(null).setPath(path);
		if (path != null) {
			//WeiXinUtils.getInstance(null).send();
		}

	}

	// 微信*******************

	private Handler wxhandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			Log.i("appactivity", msg.what + "");
			switch (msg.what) {

			case 1: {
				int result = msg.getData().getInt("result");
				Toast.makeText(m_WeiLe, result, Toast.LENGTH_LONG).show();
			}
				break;

			case 2: {
				String result = msg.getData().getString("tip");
				Toast.makeText(m_WeiLe, result, Toast.LENGTH_LONG).show();
			}
				break;

			default: {
				String result = msg.getData().getString("result");
				Toast.makeText(m_WeiLe, result, Toast.LENGTH_LONG).show();
			}
			}
		}
	};

	public Handler getWXHandler() {
		return wxhandler;
	}
}
