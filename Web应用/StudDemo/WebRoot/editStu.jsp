<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
         pageEncoding="UTF-8" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://" + request.getServerName() + ":" + request.getServerPort() + path + "/";
    String headImg = (String) session.getAttribute("headImg");
%>

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
    <h2>update Student Info</h2>
    <hr/>
    <form action="editStu" method="post">
        <s:bean name="com.stu.Action.StudentAction" id="aa"/>
        <s:fielderror value="error" cssStyle="list-style: none;color: red"/>
        <table>
            <tr hidden="hidden">
                <td>Student ID:</td>
                <td><input type="text" name="student.Id" value="${student.id}"/></td>
            </tr>
            <tr>
                <td>Student ID:</td>
                <td><input type="text" name="student.StuId" value="${student.stuId}"/></td>
            </tr>
            <tr>
                <td>Student Name:</td>
                <td><input type="text" name="student.StuName" value="${student.stuName}"/></td>
            </tr>
            <tr>
                <td>Course1 Grade:</td>
                <s:select list="#aa.grade" name="student.course1"/>
            </tr>
            <tr>
                <td>Course2 Grade:</td>
                <s:select list="#aa.grade" name="student.course2"/>
            </tr>
            <tr>
                <td>Course3 Grade:</td>
                <s:select list="#aa.grade" name="student.course3"/>
            </tr>
            <tr>
                <td>Enter Time:</td>
                <td><input readonly id="datetimepicker" type="text"
                           name="student.EnterTime" value="${student.enterTime}"/></td>
            </tr>
            <tr>
                <td>Email:</td>
                <td><input type="String" name="student.email" value="${student.email}"/></td>
            </tr>
            <tr>
                <td>head img:</td>
                <td><img style="width: 100px;height: 100px;" src="<%=basePath%><%=headImg%>"/></td>
            </tr>
            <tr>
                <td colspan="2"><input type="submit" value="submit"></td>
            </tr>
        </table>
    </form>
</center>
</body>
<script type="text/javascript">
    jQuery('#datetimepicker').datetimepicker({
        timepicker: false,
        format: 'Ymd'
    });
</script>
</html>