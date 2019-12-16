<%@ page language="java" contentType="text/html; charset=UTF-8"
         pageEncoding="UTF-8" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://" + request.getServerName() + ":" + request.getServerPort() + path + "/";
    pageContext.setAttribute("basePath", basePath);
%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="ISO-8859-1">
    <title>Insert title here</title>
    <style type="text/css">
        th {
            width: 1px;
            white-space: nowrap; /* 自适应宽度*/
            word-break: keep-all; /* 避免长单词截断，保持全部 */
            background-color: gray;
            text-color: white;
        }

        td {
            text-align: center;
        }

        table {
            table-layout: fixed;
            width: 100%;
        }

    </style>
</head>
<body>
<center>
    <h2>Student Info Manage System</h2>
    <hr/>
    <span><a href="${pageScope.basePath}/listAll">refresh</a></span>
    <span><a href="${pageScope.basePath}/addNew.jsp">add student</a></span>
    <table align="center" border="1" width="50%" cellspacing="0" cellpadding="0">
        <tr>
            <th>序列号</th>
            <th>头像</th>
            <th>学生学号</th>
            <th>学生姓名</th>
            <th>课程1成绩</th>
            <th>课程2成绩</th>
            <th>课程3成绩</th>
            <th>入学时间</th>
            <th>电子邮箱</th>
            <th>操作</th>
            <th>上传头像</th>
        </tr>
        <s:iterator value="students" status="i">
            <tr>
                <td><s:property value="Id"/></td>
                <td><img style="width: 30px;height: 30px;border-radius: 50%" src="<%=basePath%>${headImg}"></td>
                <td><s:property value="StuId"/></td>
                <td><s:property value="StuName"/></td>
                <td><s:property value="course1"/></td>
                <td><s:property value="course2"/></td>
                <td><s:property value="course3"/></td>
                <td><s:property value="EnterTime"/></td>
                <td><s:property value="email"/></td>
                <td><a href="${pageScope.basePath}/goToEdit?Id=${Id}">edit</a>
                    <a href="${pageScope.basePath}/deleteStu?Id=${Id}">delete</a></td>
                <td><a href="${pageScope.basePath}/goToUpdate?Id=${Id}">upload</a></td>
            </tr>
        </s:iterator>
    </table>
</center>
</body>
</html>