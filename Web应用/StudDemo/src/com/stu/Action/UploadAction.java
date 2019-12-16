package com.stu.Action;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;
import com.stu.Util.StudentDao;
import com.stu.model.Student;
import org.apache.struts2.ServletActionContext;

import java.io.*;

public class UploadAction extends ActionSupport {
    private String savePath;
    private File file;
    private String fileFileName;
    private String fileContentType;
    private int Id;
    private static int id;
    private Student student;

    public String getSavePath() {
        return savePath;
    }

    public void setSavePath(String savePath) {
        this.savePath = savePath;
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }

    public String getFileFileName() {
        return fileFileName;
    }

    public void setFileFileName(String fileFileName) {
        this.fileFileName = fileFileName;
    }

    public String getFileContentType() {
        return fileContentType;
    }

    public void setFileContentType(String fileContentType) {
        this.fileContentType = fileContentType;
    }

    public int getId() {
        return Id;
    }

    public void setId(int id) {
        Id = id;
    }

    public Student getStudent() {
        return student;
    }

    public void setStudent(Student student) {
        this.student = student;
    }

    public String upload() throws Exception {
        String path = ServletActionContext.getServletContext().getRealPath(savePath);
        File saveFile = new File(path);
        if (!saveFile.exists())
            saveFile.mkdir();
        String saveFilePath = path+"/" + fileFileName;
        System.out.println(saveFilePath);
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        String url = savePath + "/" + fileFileName;
        try {
            bis = new BufferedInputStream(new FileInputStream(file));
            bos = new BufferedOutputStream(new FileOutputStream(saveFilePath));
            byte[] buf = new byte[(int) file.length()];
            int len;
            while ((len = bis.read(buf)) != -1)
                bos.write(buf, 0, len);
            student = StudentDao.select(id);
            System.out.println(student);
            System.out.println(id);
            if (student != null) {
                student.setHeadImg(url);
                StudentDao.update(student);
                student = StudentDao.select(id);
                System.out.println(student);
            }
            System.out.println(student);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bis != null) bis.close();
            if (bos != null) bos.close();
        }
        return "success";
    }

    public String goToUpload() {
        student = StudentDao.select(Id);
        id = Id;
        ActionContext.getContext().getSession().put("headImg",student.getHeadImg());
        System.out.println(Id);
        return "success";
    }

}
