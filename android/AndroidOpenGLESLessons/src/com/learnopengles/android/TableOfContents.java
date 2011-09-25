package com.learnopengles.android;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.SimpleAdapter;
import android.widget.AdapterView.OnItemClickListener;

import com.learnopengles.android.lesson1.LessonOneActivity;
import com.learnopengles.android.lesson2.LessonTwoActivity;
import com.learnopengles.android.lesson3.LessonThreeActivity;
import com.learnopengles.android.lesson4.LessonFourActivity;

public class TableOfContents extends ListActivity 
{
	private static final String ITEM_IMAGE = "item_image";
	private static final String ITEM_TITLE = "item_title";
	private static final String ITEM_SUBTITLE = "item_subtitle";	
	
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setTitle(R.string.toc);
		setContentView(R.layout.table_of_contents);
		
		// Initialize data
		final List<Map<String, Object>> data = new ArrayList<Map<String, Object>>();
		final Map<Integer, Class<? extends Activity>> activityMapping = new HashMap<Integer, Class<? extends Activity>>();
		
		int i = 0;
		
		{
			final Map<String, Object> item = new HashMap<String, Object>();
			item.put(ITEM_IMAGE, R.drawable.ic_lesson_one);
			item.put(ITEM_TITLE, getText(R.string.lesson_one));
			item.put(ITEM_SUBTITLE, getText(R.string.lesson_one_subtitle));
			data.add(item);
			activityMapping.put(i++, LessonOneActivity.class);			
		}
		
		{
			final Map<String, Object> item = new HashMap<String, Object>();
			item.put(ITEM_IMAGE, R.drawable.ic_lesson_two);
			item.put(ITEM_TITLE, getText(R.string.lesson_two));
			item.put(ITEM_SUBTITLE, getText(R.string.lesson_two_subtitle));
			data.add(item);
			activityMapping.put(i++, LessonTwoActivity.class);
		}
		
		{
			final Map<String, Object> item = new HashMap<String, Object>();
			item.put(ITEM_IMAGE, R.drawable.ic_lesson_three);
			item.put(ITEM_TITLE, getText(R.string.lesson_three));
			item.put(ITEM_SUBTITLE, getText(R.string.lesson_three_subtitle));
			data.add(item);
			activityMapping.put(i++, LessonThreeActivity.class);
		}
		
		{
			final Map<String, Object> item = new HashMap<String, Object>();
			item.put(ITEM_IMAGE, R.drawable.ic_lesson_four);
			item.put(ITEM_TITLE, getText(R.string.lesson_four));
			item.put(ITEM_SUBTITLE, getText(R.string.lesson_four_subtitle));
			data.add(item);
			activityMapping.put(i++, LessonFourActivity.class);
		}
		
		final SimpleAdapter dataAdapter = new SimpleAdapter(this, data, R.layout.toc_item, new String[] {ITEM_IMAGE, ITEM_TITLE, ITEM_SUBTITLE}, new int[] {R.id.Image, R.id.Title, R.id.SubTitle});
		setListAdapter(dataAdapter);	
		
		getListView().setOnItemClickListener(new OnItemClickListener() 
		{
			@Override
			 public void onItemClick(AdapterView<?> parent, View view,
				        int position, long id) 
			{
				final Class<? extends Activity> activityToLaunch = activityMapping.get(position);
				
				if (activityToLaunch != null)
				{
					final Intent launchIntent = new Intent(TableOfContents.this, activityToLaunch);
					startActivity(launchIntent);
				}				
			}
		});
	}	
}
