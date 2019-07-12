module shift_register_mm(
    clk, reset,
    write, writedata,
    out
);

parameter datawidth = 64;

input clk, reset, write;
input [datawidth-1:0] writedata;
output out;

wire shiftin_int = 1'b0;

shift_register u0(
	.clock(clk),
	.data(writedata),
	.load(write),
	.shiftin(shiftin_int),
	.shiftout(out)
);

endmodule // shift_register_mm