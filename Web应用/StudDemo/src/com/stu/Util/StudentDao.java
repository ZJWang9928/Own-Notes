package com.stu.Util;

import com.stu.model.Student;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class StudentDao {

    public static void insert(Student student) {
        String sql = "insert into Student(StuId,StuName,Course1,Course2,Course3,EnterTime,Email) values(?,?,?,?,?,?,?)";
        JDBCUtil util = JDBCUtil.getInstance();
        util.executeUpdate(sql, new Object[]{student.getStuId(), student.getStuName(), student.getCourse1(),
                student.getCourse2(), student.getCourse3(), student.getEnterTime(), student.getEmail()});
    }

    public static void delete(int id) {
        String sql = "delete from Student where id = ?";
        JDBCUtil util = JDBCUtil.getInstance();
        util.executeUpdate(sql, new Object[]{id});
    }

    public static Student select(int Id) {
        String sql = "select * from Student where id = ?";
        JDBCUtil util = JDBCUtil.getInstance();
        List<Object> tmp = util.excuteQuery(sql, new Object[]{Id});
        HashMap<String, Object> map;
        SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd");
        Student stu;
        String StuId;
        String StuName;
        int course1;
        int course2;
        int course3;
        String EnterTime;
        String email;
        String headImg;
        for (Object obj : tmp) {
            map = (HashMap<String, Object>) obj;
            StuId = (String) map.get("StuId");
            StuName = (String) map.get("StuName");
            course1 = (Integer) map.get("Course1");
            course2 = (Integer) map.get("Course2");
            course3 = (Integer) map.get("Course3");
            EnterTime = format.format((Timestamp) (map.get("EnterTime")));
            email = (String) map.get("Email");
            headImg = (String) map.get("headImg");
            stu = new Student(StuId, StuName, course1, course2, course3, EnterTime, email);
            stu.setHeadImg(headImg);
            stu.setId(Id);
            return stu;
        }
        return null;
    }

    public static ArrayList<Student> queryAll() {
        String sql = "select * from Student";
        ArrayList<Student> res = new ArrayList<Student>();
        JDBCUtil util = JDBCUtil.getInstance();
        List<Object> tmp = util.excuteQuery(sql, null);
        HashMap<String, Object> map;
        SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd");
        Student stu;
        int Id;
        String StuId;
        String StuName;
        int course1;
        int course2;
        int course3;
        String EnterTime;
        String email;
        String headImg;
        for (Object obj : tmp) {
            map = (HashMap<String, Object>) obj;
            Id = (Integer) map.get("id");
            StuId = (String) map.get("StuId");
            StuName = (String) map.get("StuName");
            course1 = (Integer) map.get("Course1");
            course2 = (Integer) map.get("Course2");
            course3 = (Integer) map.get("Course3");
            EnterTime = format.format((Timestamp) (map.get("EnterTime")));
            email = (String) map.get("Email");
            headImg = (String) map.get("headImg");
            stu = new Student(StuId, StuName, course1, course2, course3, EnterTime, email);
            stu.setHeadImg(headImg);
            stu.setId(Id);
            res.add(stu);
        }
        return res;
    }


    public static void update(Student student) {
        String sql = "update Student set StuId = ? , StuName = ? , Course1 = ? , Course2 = ? , Course3 = ? , EnterTime = ? , Email = ? , headImg = ? where id =  ?";
        JDBCUtil util = JDBCUtil.getInstance();
        util.executeUpdate(sql, new Object[]{student.getStuId(), student.getStuName(), student.getCourse1(),
                student.getCourse2(), student.getCourse3(), student.getEnterTime(), student.getEmail(), student.getHeadImg(), student.getId()});
    }
}
