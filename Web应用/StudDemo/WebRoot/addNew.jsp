<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
         pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="ISO-8859-1">
    <title>Add Student</title>
    <link rel="stylesheet" type="text/css"
          href="css/jquery.datetimepicker.min.css"/>
    <script src="js/jquery.js"></script>
    <script src="js/jquery.datetimepicker.full.min.js"></script>
</head>
<body>
<center>
    <h2>Add New Student Info</h2>
    <hr/>
    <s:form method="post" action="addStu">
        <s:fielderror value="error" cssStyle="list-style: none;color: red"/>
        <s:bean id="aa" name="com.stu.Action.StudentAction"/>
        <s:textfield name="student.StuId" label="Student ID"/>
        <s:textfield name="student.StuName" label="Student Name"/>
        <s:select list="#aa.grade" name="student.course1" label="Course1 Grade"/>
        <s:select list="#aa.grade" name="student.course2" label="Course2 Grade"/>
        <s:select list="#aa.grade" name="student.course3" label="Course3 Grade"/>
        <s:textfield readonly="true" value="20000101" cssClass="datetimepicker" name="student.EnterTime"
                     label="EnterTime"/>
        <s:textfield name="student.email" label="Email"/>
        <s:reset value="重置" align="left"/>
        <s:submit value="登陆" align="left"/>
    </s:form>
</center>
</body>
<script type="text/javascript">
    jQuery('.datetimepicker').datetimepicker({
        timepicker: false,
        format: 'Ymd'
    });
</script>
</html>