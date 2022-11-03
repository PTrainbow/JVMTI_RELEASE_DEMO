package com.rain.sad.jvmti

import android.content.Context
import android.os.Build
import android.os.Debug
import androidx.annotation.RequiresApi
import java.io.File
import java.nio.file.Files
import java.nio.file.Paths

/**
 * author: jianyi
 * date: 2022/11/3
 */
object JvmtiManager {

    @RequiresApi(Build.VERSION_CODES.P)
    fun attach(context: Context) : Boolean{
        try {
            val path: String = createDuplicateLib(context)
            System.load(path)
            nInitInReleaseMode()
            Debug.attachJvmtiAgent(path, null, context.classLoader)
        } catch (e: Exception) {
            return false
        }
        return true
    }


    /**
     *
     * 在 attachJvmtiAgent 前调用
     * nInitReleaseMode();
     * Debug.attachJvmtiAgent(native_lib_path, options, classloader);
     */
    private external fun nInitInReleaseMode()


    /**
     * 从各种开源的 jvmti demo 中 copy 而来
     * 这里必须要加载到自己的应用目录下
     *
     */
    @RequiresApi(Build.VERSION_CODES.P)
    private fun createDuplicateLib(context: Context): String {
        try {
            val findLibrary =
                ClassLoader::class.java.getDeclaredMethod("findLibrary", String::class.java)
            val apkSoPath =
                findLibrary.invoke(context.classLoader, "jvmti") as String

            val destDir = File(context.filesDir, "jvmti")
            if (!destDir.exists()) {
                destDir.mkdirs()
            }
            val destSoFile = File(destDir, "jvmti.so")
            if (destSoFile.exists()) {
                destSoFile.delete()
            }
            Files.copy(Paths.get(apkSoPath), Paths.get(destSoFile.absolutePath))
            return destSoFile.absolutePath
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return ""
    }
}