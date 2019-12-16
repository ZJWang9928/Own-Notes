package com.stu.test;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import com.stu.Util.StudentDao;
import com.stu.model.Student;

public class JDBCTest {

	public static void main(String[] args) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Student s1 = new Student("1405150116","wangyu",99,87,92,dateFormat.format(new Date()),"wangyu19970819@gmail.com");
//		StudentDao.insert(s1);
//		StudentDao.delete(3);
		s1.setId(3);
		s1.setStuName("wy2");
		s1.setStuId("1405150118");
		StudentDao.update(s1);
		ArrayList<Student> res = StudentDao.queryAll();
		for(Student stu:res)
			System.out.println(stu);
	}
}
