module.exports = {
    entry: './src/index.js',
    output: {
        filename: 'index.js',
        path: `${__dirname}`
    },
    module: {
        rules: [
            {
                test: /\.svelte$/,
                use: 'svelte-loader',
            }
        ]
    }
}