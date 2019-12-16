package com.stu.Action;

import com.opensymphony.xwork2.ActionContext;

import com.opensymphony.xwork2.ActionSupport;
import com.stu.Util.StudentDao;
import com.stu.model.Student;
import org.apache.struts2.ServletActionContext;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class StudentAction extends ActionSupport {

    private static final long serialVersionUID = 1L;

    private List<Integer> grade;
    private Student student = new Student();
    private ArrayList<Student> students = new ArrayList<Student>();
    private int Id;

    public StudentAction() {
        grade = new ArrayList<Integer>();
        for (int i = 0; i <= 100; i++)
            grade.add(i);
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

    public List<Integer> getGrade() {
        return grade;
    }

    public void setGrade(List<Integer> grade) {
        this.grade = grade;
    }

    public void setStudent(Student student) {
        this.student = student;
    }

    public ArrayList<Student> getStudents() {
        return students;
    }

    public void setStudents(ArrayList<Student> students) {
        this.students = students;
    }

    public String listAll() {
        students = StudentDao.queryAll();
        System.out.println(students.size());
        return "success";
    }

    public String addStu() {
        System.out.println(student);
        StudentDao.insert(student);
        return "success";
    }

    public String editStu() {
        System.out.println(student);
        StudentDao.update(student);
        return "success";
    }

    public String goToEdit() {
        System.out.println(Id);
        student = StudentDao.select(Id);
        System.out.println(student);
        ActionContext.getContext().getSession().put("headImg",student.getHeadImg());
        return "success";
    }

    public String deleteStu() {
        StudentDao.delete(Id);
        return "success";
    }

    public void validateAddStu() {
        vali();
    }

    public void validateEditStu() {
        vali();
    }


    private void vali() {
        String username = student.getStuName();
        String studentId = student.getStuId();
        String email = student.getEmail();
        if (studentId == null || studentId.isEmpty())
            this.addFieldError("error", "Student Id cannot be empty");
        else if (!Pattern.matches("^[a-zA-Z][a-zA-Z0-9_]{3,14}$", studentId))
            this.addFieldError("error", "Student Id must start with character\n and can follow with" +
                    " digital  character or underline which length between 4 and 15");
        else if (username == null || username.isEmpty())
            this.addFieldError("error", "Student Name cannot be empty");
        else if (email == null || email.isEmpty())
            this.addFieldError("error", "email cannot be empty");
        else if (!Pattern.matches("^[A-Za-z0-9\\u4e00-\\u9fa5]+@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)+$", email))
            this.addFieldError("error", "email address illegal");
    }

}
