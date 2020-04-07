#include"win.h"
BOOL LoadNTDriver(LPCWSTR lpszDriverName, LPCWSTR lpszDriverPath) {
    {
        WCHAR szDriverImagePath[256];
        //�õ�����������·��  
        GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
        BOOL bRet = FALSE;
        SC_HANDLE hServiceMgr = NULL;//SCM�������ľ��  
        SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����  
        //�򿪷�����ƹ�����  
        hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (hServiceMgr == NULL)
        {
            //OpenSCManagerʧ��
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
            ////OpenSCManager�ɹ�  
            ;// printf("OpenSCManager() ok ! \n");
        }
        //������������Ӧ�ķ���  
        hServiceDDK = CreateService(hServiceMgr,
            lpszDriverName, //�����������ע����е�����    
            lpszDriverName, // ע������������ DisplayName ֵ    
            SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��    
            SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������    
            SERVICE_DEMAND_START, // ע������������ Start ֵ    
            SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ    
            szDriverImagePath, // ע������������ ImagePath ֵ    
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
        DWORD dwRtn;
        //�жϷ����Ƿ�ʧ��  
        if (hServiceDDK == NULL)
        {
            dwRtn = GetLastError();
            if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
            {
                //��������ԭ�򴴽�����ʧ��  
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
                //���񴴽�ʧ�ܣ������ڷ����Ѿ�������
                QString str1 = "Service already exited ";
                QMessageBox::information(NULL, "info", str1, QMessageBox::Yes, QMessageBox::Yes);
                
                //printf("CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
            }
            // ���������Ѿ�����,�����Ѿ�������ֻ��Ҫ��    
            hServiceDDK = OpenService(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
            if (hServiceDDK == NULL)
            {
                //����򿪷���Ҳʧ�ܣ�����ζ����  
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
        //�����������  
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
                    //�豸����ס  
                    QMessageBox::information(NULL, "Failed", "StartService() Faild ERROR_IO_PENDING !", QMessageBox::Yes, QMessageBox::Yes);
                   // printf("StartService() Faild ERROR_IO_PENDING ! \n");
                    bRet = FALSE;
                    goto BeforeLeave;
                }
                else
                {
                    //�����Ѿ�����  
                   // printf("StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
                    bRet = TRUE;
                    goto BeforeLeave;
                }
            }
        }
        else {
            ;// printf("StartService() success! \n");
        }
        //�뿪ǰ�رվ��  
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
    SC_HANDLE hServiceMgr = NULL;//SCM�������ľ��  
    SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����  
    SERVICE_STATUS SvrSta;
    //��SCM������  
    hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hServiceMgr == NULL)
    {
        //OpenSCManagerʧ��
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
        //����SCM������ʧ�ܳɹ�  
        ;// printf("OpenSCManager() ok ! \n");
    }
    //����������Ӧ�ķ���  
    hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
    if (hServiceDDK == NULL)
    {
        //����򿪷���Ҳʧ�ܣ�����ζ����  
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
    //ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�    
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
    //��̬ж����������    
    if (!DeleteService(hServiceDDK))
    {
        //ж��ʧ��  
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
        //ж�سɹ�  
        ;// printf("DelServer:eleteSrevice() ok !\n");
    }
    bRet = TRUE;
BeforeLeave:
    //�뿪ǰ�رմ򿪵ľ��  
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