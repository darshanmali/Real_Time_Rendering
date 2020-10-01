



inside forloop

{
    CLGetPlatformInfo()

    printf("%s",info);
    //this will we do for device count for respective platform 

    Rateocl = clGetDiveceIDs(cl_Platform_id[i],CL_DEVICE_TYPE_ALL,0,NULL,&Device_count);
    if (rateocl < 1)
    {
        printf("give Message");
        continue;
       // exit(EXIT_FAILER); caouse another platform can have Device
    }

    cl_Device_ids = (CL_Device_IDs *) malloc(Device_count * sizeof(cl_Device_IDs));
    // here you sould do error chackeing 

    Rateocl = CLGetDevice_IDs(CL_PLATFORM_DEVICE,CL_DEVICE_TYPE_ALL,Device_count,cl_Device_ids,null);

    //whereever we got Rate_ocl we should do error Chacking 
    /*if (Rate_ocl != CL_Success)
    {

    }*/
    for(int j = 0; j< Device_count; j++)
    {
        CLGetDeviceInfo(CL_DEvice_ids[j],CL_DeviceName,512,info,NULL);
        printf("%s Device",info);


        //Declare CL_DEVICE_TYPE device_type in globally 
        CLGetDeviceInfo(CL_Device_ids[j],CL_DEVICE_TYPE,sizeof(CL_DEVICE_TYPE), &DEvice_type,NULL);


        //Now decide thetype of device you want 
        if(Device_type !& CL_Device_TYPE_CPU)
        {
            ocl_Platform_required = CL_DEVICE_Id[j];
            break;

        }
    }
//completing the For Loop of j 
    free(cl_Device_ids);
    break;
}
//completing the For Loop of i 
free(CL_PLATFORM_ids);
}


