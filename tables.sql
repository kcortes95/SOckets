create table cancelados(
	id_cliente integer not null,
	id_vuelo integer not null,
	f integer not null,
	c integer not null,
	primary key (id_cliente, id_vuelo, f, c),
	foreign key (id_cliente) references cliente(id),
	foreign key (id_vuelo) references vuelos(id)
);

create table cliente(
	id integer primary key not null,
	nombre text,
	apellido text
);

create table vuelos(
	id_vuelo integer primary key not null,
	aerolinea text,	
	desde text,
	hacia text
);

create table asientos(
	id_vuelo integer not null,
	f integer not null,
	c integer not null,
	fecha text,
	estado text,
	id_cliente integer,
	primary key(id_vuelo,f,c, fecha),
	foreign key (id_cliente) references cliente(id),
	foreign key (id_vuelo) references vuelos(id)
);