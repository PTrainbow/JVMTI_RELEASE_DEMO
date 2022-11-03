package com.rain.sad.jvmti_release

import android.os.Build
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.rain.sad.jvmti.JvmtiManager
import com.rain.sad.jvmti_release.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.sampleText.setOnClickListener {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                JvmtiManager.attach(this@MainActivity)
            }
        }
    }

}