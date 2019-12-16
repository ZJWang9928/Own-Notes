package com.stu.model;

public class Student {
    private int id;
    private String StuId;
    private String StuName;
    private int course1;
    private int course2;
    private int course3;
    private String EnterTime;
    private String email;
    private String headImg;

    public Student() {
    }



    public Student(String StuId, String StuName, int course1, int course2, int course3, String EnterTime, String email) {
        this.StuId = StuId;
        this.StuName = StuName;
        this.course1 = course1;
        this.course2 = course2;
        this.course3 = course3;
        this.EnterTime = EnterTime;
        this.email = email;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getStuId() {
        return StuId;
    }

    public void setStuId(String stuId) {
        StuId = stuId;
    }

    public String getStuName() {
        return StuName;
    }

    public void setStuName(String stuName) {
        StuName = stuName;
    }

    public int getCourse1() {
        return course1;
    }

    public void setCourse1(int course1) {
        this.course1 = course1;
    }

    public int getCourse2() {
        return course2;
    }

    public void setCourse2(int course2) {
        this.course2 = course2;
    }

    public int getCourse3() {
        return course3;
    }

    public void setCourse3(int course3) {
        this.course3 = course3;
    }

    public String getEnterTime() {
        return EnterTime;
    }

    public void setEnterTime(String enterTime) {
        EnterTime = enterTime;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getHeadImg() {
        return headImg;
    }
    public void setHeadImg(String headImg) {
        this.headImg = headImg;
    }

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", StuId='" + StuId + '\'' +
                ", StuName='" + StuName + '\'' +
                ", course1=" + course1 +
                ", course2=" + course2 +
                ", course3=" + course3 +
                ", EnterTime='" + EnterTime + '\'' +
                ", email='" + email + '\'' +
                ", headImg='" + headImg + '\'' +
                '}';
    }
}
