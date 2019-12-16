<%@ taglib prefix="s" uri="/struts-tags" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://" + request.getServerName() + ":" + request.getServerPort() + path + "/";
    String headImg = (String) session.getAttribute("headImg");
%>

<%--
  Created by IntelliJ IDEA.
  User: wangyu
  Date: 2018/10/10
  Time: 10:26 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>File Upload</title>
    <script src="js/jquery.js"></script>
</head>
<body>
<center>
    <h3>Image Upload</h3>
    <hr/>
    <form action="upload.action" method="post" enctype="multipart/form-data">
        <label for="file">Upload your image :</label>
        <input type="file" id="file" name="file"/>
        <label style="display: block;" for="file">
            <img style="width: 400px;height: 400px;" id="Img" src="<%=basePath%><%=headImg%>"/>
        </label>
        <input type="submit" value="Upload"/>
    </form>
</center>
</body>
<script type="text/javascript">
    $('#file').change(function () {
        $('p').css('display', 'none');
        var objUrl = getObjectURL(this.files[0]);
        if (objUrl) {
            $('#Img').attr("src", objUrl);
        }

        function getObjectURL(file) {
            var url = null;
            if (window.createObjectURL !== undefined) {
                url = window.createObjectURL(file);
            } else window.webkitURL = undefined;
            if (window.URL !== undefined) {
                url = window.URL.createObjectURL(file);
            } else if (window.webkitURL !== undefined) {
                url = window.webkitURL.createObjectURL(file);
            }
            return url;
        }
    });
</script>
</html>
