#define NUM_OF_ELECTRODES	13



/* turn off all electrodes to stop */
		write_register(ECR, 0x00); 

		/* write register with initial values */
		write_register(MHDR, 0x01);
		write_register(NHDR, 0x01);
		write_register(NCLR, 0x10);
		write_register(FDLR, 0x20);
		write_register(MHDF, 0x01);
		write_register(NHDF, 0x01);
		write_register(NCLF, 0x10);
		write_register(FDLF, 0x20);
		write_register(NHDT, 0x01);
		write_register(NCLT, 0x10);
		write_register(FDLT, 0xFF);
		write_register(MHDPROXR, 0x0F);
		write_register(NHDPROXR, 0x0F);
		write_register(NCLPROXR, 0x00);
		write_register(FDLPROXR, 0x00);
		write_register(MHDPROXF, 0x01);
		write_register(NHDPROXF, 0x01);
		write_register(NCLPROXF, 0xFF);
		write_register(FDLPROXF, 0xFF);
		write_register(NHDPROXT, 0x00);
		write_register(NCLPROXT, 0x00);
		write_register(FDLPROXT, 0x00);
	  	write_register(DTR, 0x11);
		write_register(AFE1, 0xFF);  
		write_register(AFE2, 0x30);
		write_register(ACCR0, 0x00);
		write_register(ACCR1, 0x00);
		write_register(USL, 0x00); 
		write_register(LSL, 0x00); 
		write_register(TL, 0x00); 
		write_register(ECR, 0xCC); // default to fast baseline startup and 12 electrodes enabled, no prox
	
		/* apply next setting for all electrodes */
		for (electrodes_count = 0; electrodes_count < NUM_OF_ELECTRODES; electrodes_count++) {
			write_register((E0TTH + (electrodes_count<<1)), 40); 
			write_register((E0RTH + (electrodes_count<<1)), 20); 
		}

		/* enable electrodes and set the current to 16uA */
		write_register(ECR, 0x10); 
		