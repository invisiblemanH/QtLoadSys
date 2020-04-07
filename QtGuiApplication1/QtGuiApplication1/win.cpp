#include"win.h"
BOOL LoadNTDriver(LPCWSTR lpszDriverName, LPCWSTR lpszDriverPath) {
    {
        WCHAR szDriverImagePath[256];
        //得到完整的驱动路径  
        GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
        BOOL bRet = FALSE;
        SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄  
        SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄  
        //打开服务控制管理器  
        hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (hServiceMgr == NULL)
        {
            //OpenSCManager失败
            QString str1 = "OpenSCManager() Faild! GetLastError() :";
            DWORD n = GetLastError();
            QString str2;
            str2 = QString::number(n);
            QString str = str1 + str2;
            QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
            //printf("OpenSCManager() Faild %d ! \n", GetLastError());
            bRet = FALSE;
            goto BeforeLeave;
        }
        else
        {
            ////OpenSCManager成功  
            ;// printf("OpenSCManager() ok ! \n");
        }
        //创建驱动所对应的服务  
        hServiceDDK = CreateService(hServiceMgr,
            lpszDriverName, //驱动程序的在注册表中的名字    
            lpszDriverName, // 注册表驱动程序的 DisplayName 值    
            SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限    
            SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序    
            SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值    
            SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值    
            szDriverImagePath, // 注册表驱动程序的 ImagePath 值    
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
        DWORD dwRtn;
        //判断服务是否失败  
        if (hServiceDDK == NULL)
        {
            dwRtn = GetLastError();
            if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
            {
                //由于其他原因创建服务失败  
                QString str1 = "CrateService() Faild! GetLastError() :";
                DWORD n = GetLastError();
                QString str2;
                str2 = QString::number(n);
                QString str = str1 + str2;
                QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
                //printf("OpenSCManager() Faild %d ! \n", GetLastError());
                bRet = FALSE;
                goto BeforeLeave;
            }
            else
            {
                //服务创建失败，是由于服务已经创立过
                QString str1 = "Service already exited ";
                QMessageBox::information(NULL, "info", str1, QMessageBox::Yes, QMessageBox::Yes);
                
                //printf("CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
            }
            // 驱动程序已经加载,服务已经创建，只需要打开    
            hServiceDDK = OpenService(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
            if (hServiceDDK == NULL)
            {
                //如果打开服务也失败，则意味错误  
                QString str1 = "OpenService() Faild! GetLastError() :";
                DWORD n = GetLastError();
                QString str2;
                str2 = QString::number(n);
                QString str = str1 + str2;
                QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
                //printf("OpenSCManager() Faild %d ! \n", GetLastError());
                bRet = FALSE;
                goto BeforeLeave;
            }
            else
            {
                ;// printf("OpenService() ok ! \n");
            }
        }
        else
        {
            ; printf("CrateService() ok ! \n");
        }
        //开启此项服务  
        bRet = StartService(hServiceDDK, NULL, NULL);
        if (!bRet)
        {
            if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING)
            {
                QString str1 = "StartService() Faild! GetLastError() :";
                DWORD n = GetLastError();
                QString str2;
                str2 = QString::number(n);
                QString str = str1 + str2;
                QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
                //printf("OpenSCManager() Faild %d ! \n", GetLastError());
                bRet = FALSE;
                goto BeforeLeave;
                
               
            }
            else
            {
                if (dwRtn == ERROR_IO_PENDING)
                {
                    //设备被挂住  
                    QMessageBox::information(NULL, "Failed", "StartService() Faild ERROR_IO_PENDING !", QMessageBox::Yes, QMessageBox::Yes);
                   // printf("StartService() Faild ERROR_IO_PENDING ! \n");
                    bRet = FALSE;
                    goto BeforeLeave;
                }
                else
                {
                    //服务已经开启  
                   // printf("StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
                    bRet = TRUE;
                    goto BeforeLeave;
                }
            }
        }
        else {
            ;// printf("StartService() success! \n");
        }
        //离开前关闭句柄  
    BeforeLeave:
        if (hServiceDDK)
        {
            CloseServiceHandle(hServiceDDK);
        }
        if (hServiceMgr)
        {
            CloseServiceHandle(hServiceMgr);
        }
        return bRet;
    }
}
BOOL UnloadNTDriver(LPCWSTR szSvrName)
{
    BOOL bRet = FALSE;
    SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄  
    SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄  
    SERVICE_STATUS SvrSta;
    //打开SCM管理器  
    hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hServiceMgr == NULL)
    {
        //OpenSCManager失败
        QString str1 = "OpenSCManager() Faild! GetLastError() :";
        DWORD n = GetLastError();
        QString str2;
        str2 = QString::number(n);
        QString str = str1 + str2;
        QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
        //printf("OpenSCManager() Faild %d ! \n", GetLastError());
        bRet = FALSE;
        goto BeforeLeave;
    }
    else
    {
        //带开SCM管理器失败成功  
        ;// printf("OpenSCManager() ok ! \n");
    }
    //打开驱动所对应的服务  
    hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
    if (hServiceDDK == NULL)
    {
        //如果打开服务也失败，则意味错误  
        QString str1 = "OpenService() Faild! GetLastError() :";
        DWORD n = GetLastError();
        QString str2;
        str2 = QString::number(n);
        QString str = str1 + str2;
        QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
        //printf("OpenSCManager() Faild %d ! \n", GetLastError());
        bRet = FALSE;
        goto BeforeLeave;
    }
    else
    {
        ;// printf("OpenService() ok ! \n");
    }
    //停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。    
    if (!ControlService(hServiceDDK, SERVICE_CONTROL_STOP, &SvrSta))
    {
        QString str1 = "ControlService() Faild! GetLastError() :";
        DWORD n = GetLastError();
        QString str2;
        str2 = QString::number(n);
        QString str = str1 + str2;
        QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
        bRet = FALSE;
        goto BeforeLeave;
    }
    else
    {
        ;// printf("ControlService() ok !\n");
    }
    //动态卸载驱动程序。    
    if (!DeleteService(hServiceDDK))
    {
        //卸载失败  
        QString str1 = "DeleteSrevice() Faild! GetLastError() :";
        DWORD n = GetLastError();
        QString str2;
        str2 = QString::number(n);
        QString str = str1 + str2;
        QMessageBox::information(NULL, "Failed", str, QMessageBox::Yes, QMessageBox::Yes);
        bRet = FALSE;
        goto BeforeLeave;
    }
    else
    {
        //卸载成功  
        ;// printf("DelServer:eleteSrevice() ok !\n");
    }
    bRet = TRUE;
BeforeLeave:
    //离开前关闭打开的句柄  
    if (hServiceDDK)
    {
        CloseServiceHandle(hServiceDDK);
    }
    if (hServiceMgr)
    {
        CloseServiceHandle(hServiceMgr);
    }
    return bRet;
}