create table cliente(
	dni      text primary key not null,
	nombre   text,
	apellido text
);

create table vuelos(
	id_vuelo integer primary key not null,
	desde    text not null,
	hacia    text not null,
	estado   text not null
);

create table reserva(
	id_vuelo   integer not null,
	id_cliente integer not null,
	fil        integer not null,
	col        integer not null,
	fecha      text    not null,
	primary key(id_vuelo,id_cliente),
	foreign key (id_cliente) references cliente(id),
	foreign key (id_vuelo) references vuelos(id)
);