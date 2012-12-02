/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Thomas Martitz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

package org.rockbox;


import org.rockbox.Helper.Logger;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.ResultReceiver;
import android.text.InputType;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.Toast;

import com.umeng.analytics.MobclickAgent;
import com.umeng.fb.NotificationType;
import com.umeng.fb.UMFeedbackService;

public class RockboxActivity extends Activity 
{
    private RockboxApp RockboxAppSetting = RockboxApp.getInstance();
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        UMFeedbackService.enableNewReplyNotification(this, NotificationType.AlertDialog);
	Toast.makeText(this, "Rockbox中文社区精心定制", Toast.LENGTH_LONG).show();
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
        Intent intent = new Intent(this, RockboxService.class);
        intent.setAction(Intent.ACTION_MAIN);
        intent.putExtra("callback", new ResultReceiver(new Handler(getMainLooper())) {
            private boolean unzip = false;
            private ProgressDialog loadingdialog;
            private void createProgressDialog()
            {
                loadingdialog = new ProgressDialog(RockboxActivity.this);
                loadingdialog.setMessage(getString(R.string.rockbox_extracting));
                loadingdialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
                loadingdialog.setIndeterminate(true);
                loadingdialog.setCancelable(false);
                loadingdialog.show();
            }

            @Override
            protected void onReceiveResult(final int resultCode, final Bundle resultData)
            {
                RockboxFramebuffer fb;
                switch (resultCode) {
                    case RockboxService.RESULT_INVOKING_MAIN:
                        if (loadingdialog != null)
                            loadingdialog.dismiss();
                        fb = new RockboxFramebuffer(RockboxActivity.this);
                        setContentView(fb);
                        fb.requestFocus();
                        break;
                    case RockboxService.RESULT_LIB_LOAD_PROGRESS:
                        if (loadingdialog == null)
                            createProgressDialog();
                        loadingdialog.setIndeterminate(false);
                        loadingdialog.setMax(resultData.getInt("max", 100));
                        loadingdialog.setProgress(resultData.getInt("value", 0));
                        break;
                    case RockboxService.RESULT_LIB_LOADED:
                        unzip = resultData.getBoolean("unzip");
                        break;
                    case RockboxService.RESULT_SERVICE_RUNNING:
                        if (!unzip) /* defer to RESULT_INVOKING_MAIN */
                        {
                            fb = new RockboxFramebuffer(RockboxActivity.this);
                            setContentView(fb);
                            fb.requestFocus();
                        }
                        setServiceActivity(true);
                        break;
                    case RockboxService.RESULT_ERROR_OCCURED:
                        Toast.makeText(RockboxActivity.this, resultData.getString("error"), Toast.LENGTH_LONG).show();
                        break;
                    case RockboxService.RESULT_ROCKBOX_EXIT:
                        finish();
                        break;
                }
            }
        });
        startService(intent);
    }
    
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        menu.clear();
        if (!RockboxAppSetting.getRockboxWakeLockStatus())
        menu.add(0, 2, 0, R.string.rockbox_wakelock_on);
        if (RockboxAppSetting.getRockboxWakeLockStatus())
        menu.add(0, 3, 0, R.string.rockbox_wakelock_off);
        menu.add(0, 5, 0, R.string.rockbox_vollock_on);
        if (RockboxAppSetting.getRockboxVolLockStatus())
        menu.add(0, 6, 0, R.string.rockbox_vollock_off);
        menu.add(0, 0, 0, R.string.UMFeedbackUmengTitle);
        menu.add(0, 4, 0, R.string.rockbox_about);
        menu.add(0, 1, 0, R.string.rockbox_exit);
        menu.add(0, 7, 0, R.string.rockbox_simulatebutton);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        final EditText InputVol=new EditText(this);
        InputVol.setInputType(InputType.TYPE_CLASS_NUMBER);
        if (RockboxAppSetting.getRockboxVolLockStatus())
        InputVol.setHint(getResources().getString(R.string.rockbox_vollock_hint1)+Integer.toString(RockboxAppSetting.getVol()));
        else
        InputVol.setHint(getResources().getString(R.string.rockbox_vollock_hint1)+Integer.toString(RockboxAppSetting.getVol())+getResources().getString(R.string.rockbox_vollock_hint2));
        switch (item.getItemId())
        {
            case 0:
                UMFeedbackService.setGoBackButtonVisible();
                UMFeedbackService.openUmengFeedbackSDK(this);
                break;
            case 1:
                System.runFinalization();
                android.os.Process.killProcess(android.os.Process.myPid());
                System.exit(0);
                break;
            case 2:
                RockboxAppSetting.setRockboxWakeLockStatus(true);
                RockboxAppSetting.SaveSetting(1, false, false, true, true);
                Toast.makeText(this, R.string.rockbox_wakelock_on_toast, Toast.LENGTH_LONG).show();
                break;
            case 3:
                RockboxAppSetting.setRockboxWakeLockStatus(false);
                RockboxAppSetting.SaveSetting(1, false, false, false, true);
                Toast.makeText(this, R.string.rockbox_wakelock_off_toast, Toast.LENGTH_LONG).show();
                break;
            case 4:
                new AlertDialog.Builder(this)
            	                .setTitle(R.string.rockbox_about_title)
            	                .setMessage(R.string.rockbox_about_message)
            	                .setPositiveButton(R.string.OK, null)
            	                .show();
                break;
            case 5:
                new AlertDialog.Builder(this)
            	                .setTitle(R.string.rockbox_vollock_info)
            	                .setView(InputVol)
            	                .setPositiveButton(R.string.OK, new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int whichButton) {
                                    	try{
                                            int voltmp=Integer.parseInt(InputVol.getText().toString());
                                            if (voltmp <= 0)
                                            voltmp = 1;
                                            if (voltmp >= 101)
                                            voltmp = 100;
                                            RockboxAppSetting.setRockboxVolLockStatus(true);
                                            RockboxAppSetting.SaveSetting(voltmp, true, true, false, false);
                                            RockboxAppSetting.setVol(voltmp);
                                            Toast.makeText(RockboxActivity.this,R.string.rockbox_vollock_toast, Toast.LENGTH_LONG).show();
                                    	    }catch (Exception e){
                                    	        Logger.d("Volume-Lock input error!");
                                    	        Toast.makeText(RockboxActivity.this, R.string.rockbox_vollock_inputerror, Toast.LENGTH_LONG).show();
                                    	    }
                                        }
                                    })
                                .setNegativeButton(R.string.Cancel, null)
                                .show();
                break;
            case 6:
            	RockboxAppSetting.setRockboxVolLockStatus(false);
            	RockboxAppSetting.SaveSetting(1, false, true, false, false);
            	Toast.makeText(this, R.string.rockbox_vollock_off_toast, Toast.LENGTH_LONG).show();
            	break;
            case 7:
                RockboxFramebuffer.buttonHandler(0, true); //press
                try {
                    Thread.sleep(400); //线程阻塞400ms，模拟长按菜单键（WPS_MENU），小于300ms即为WPS_CONTEXT。
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                    Logger.e("Thread blocked!");
                    }
                RockboxFramebuffer.buttonHandler(0, false); //release
                break;
         }
        return true;
     }

    private void setServiceActivity(boolean set)
    {
        RockboxService s = RockboxService.getInstance();
        if (s != null)
            s.setActivity(set ? this : null);
    }

    public void onResume()
    {
        super.onResume();
        RockboxAppSetting.releaseWakeLock();
        MobclickAgent.onResume(this);
        setVisible(true);
    }
    
    /* this is also called when the backlight goes off,
     * which is nice 
     */
    @Override
    protected void onPause() 
    {
        super.onPause();
        RockboxAppSetting.acquireWakeLock();
        MobclickAgent.onPause(this);
        /* this will cause the framebuffer's Surface to be destroyed, enabling
         * us to disable drawing */
        setVisible(false);
    }
    
    @Override
    protected void onStop() 
    {
        super.onStop();
        setServiceActivity(false);
    }
    
    @Override
    protected void onDestroy() 
    {
        super.onDestroy();
        setServiceActivity(false);
    }
}
